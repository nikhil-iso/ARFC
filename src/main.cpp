#include <Arduino.h>

#include "config/AppConfig.h"
#include "data/FlightSample.h"
#include "datasource/HILSensorSource.h"
#include "datasource/IDataSource.h"
#include "detection/EventDetector.h"
#include "detection/FlightStateMachine.h"
#include "logging/Logger.h"
#include "status/StatusManager.h"

namespace {

arfc::StatusManager g_status_manager;
arfc::Logger g_logger;
arfc::HILSensorSource g_hil_sensor_source(Serial);
arfc::EventDetector g_event_detector;
arfc::FlightStateMachine g_flight_state_machine;
arfc::IDataSource* g_data_source = nullptr;
bool g_data_source_ready = false;
bool g_logger_error_reported = false;
bool g_packet_error_reported = false;

void applyEventFlags(arfc::FlightSample& sample, const arfc::EventFlags& events) {
    sample.launch_flag = events.launch_flag;
    sample.apogee_flag = events.apogee_flag;
    sample.descent_flag = events.descent_flag;
    sample.landing_flag = events.landing_flag;
}

void printSampleDebug(const arfc::FlightSample& sample, arfc::FlightState state) {
    Serial.print("State=");
    Serial.print(arfc::toString(state));
    Serial.print(" t=");
    Serial.print(sample.timestamp_ms);
    Serial.print(" rel=");
    Serial.print(sample.rel_altitude_m, 2);
    Serial.print(" amag=");
    Serial.print(sample.accel_mag_g, 3);
    Serial.print(" flags=");
    Serial.print(sample.launch_flag ? 'L' : '-');
    Serial.print(sample.apogee_flag ? 'A' : '-');
    Serial.print(sample.descent_flag ? 'D' : '-');
    Serial.println(sample.landing_flag ? 'N' : '-');
}

}  // namespace

void setup() {
    Serial.begin(115200);
    g_status_manager.begin();
    g_status_manager.setFlightState(arfc::FlightState::BOOT);
    Serial.println("ARFC booting...");
    g_event_detector.reset();
    g_flight_state_machine.reset();
    g_flight_state_machine.begin();
    g_status_manager.setFlightState(g_flight_state_machine.state());

    if (arfc::AppConfig::kDataSourceMode == arfc::DataSourceMode::MODE_HIL) {
        g_data_source = &g_hil_sensor_source;
    } else {
        g_status_manager.setError(true);
        Serial.println("MODE_LIVE not implemented");
    }

    if (g_data_source != nullptr && g_data_source->begin()) {
        g_data_source_ready = true;
        Serial.print("Data source ready: ");
        Serial.println(g_data_source->name());
    } else if (g_data_source != nullptr) {
        g_status_manager.setError(true);
        Serial.println("Data source init failed");
    }

    if (g_logger.begin()) {
        Serial.print("Logger ready: ");
        Serial.println(g_logger.fileName());
    } else {
        g_status_manager.setError(true);
        g_logger_error_reported = true;
        Serial.println("Logger init failed");
    }
}

void loop() {
    const uint32_t now_ms = millis();
    g_status_manager.update(now_ms);

    if (!g_logger.isReady() || !g_data_source_ready) {
        return;
    }

    arfc::FlightSample sample;
    if (!g_data_source->readSample(sample)) {
        if (g_hil_sensor_source.lastStatus() == arfc::HILReadStatus::InvalidPacket &&
            !g_packet_error_reported) {
            Serial.println("HIL packet rejected");
            g_packet_error_reported = true;
        }
        return;
    }

    g_packet_error_reported = false;
    const arfc::EventFlags events = g_event_detector.update(sample);
    applyEventFlags(sample, events);
    const arfc::FlightState state = g_flight_state_machine.update(sample, events);
    g_status_manager.setFlightState(state);

    if (!g_logger.logSample(sample)) {
        g_status_manager.setError(true);
        if (!g_logger_error_reported) {
            Serial.println("Logger write failed");
            g_logger_error_reported = true;
        }
        return;
    }

    g_logger_error_reported = false;
    printSampleDebug(sample, state);
}
