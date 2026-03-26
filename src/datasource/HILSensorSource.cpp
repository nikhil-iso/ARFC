#include "datasource/HILSensorSource.h"

#include <math.h>

namespace arfc {

namespace {

constexpr float kSeaLevelPressurePa = 101325.0f;
constexpr float kAltitudeExponent = 0.19029495f;

}  // namespace

HILSensorSource::HILSensorSource(Stream& serial_port)
    : serial_port_(serial_port),
      altitude_baseline_initialized_(false),
      altitude_baseline_m_(0.0f),
      last_status_(HILReadStatus::NoData) {}

bool HILSensorSource::begin() {
    altitude_baseline_initialized_ = false;
    altitude_baseline_m_ = 0.0f;
    last_status_ = HILReadStatus::NoData;
    return true;
}

bool HILSensorSource::readSample(FlightSample& sample) {
    HILPacket packet;
    const SerialPacketStatus packet_status = parser_.readPacket(serial_port_, packet);

    if (packet_status == SerialPacketStatus::NoPacket) {
        last_status_ = HILReadStatus::NoData;
        return false;
    }

    if (packet_status == SerialPacketStatus::InvalidPacket) {
        last_status_ = HILReadStatus::InvalidPacket;
        return false;
    }

    sample = FlightSample{};
    sample.timestamp_ms = packet.timestamp_ms;
    sample.pressure_pa = packet.pressure_pa;
    sample.temperature_c = packet.temperature_c;
    sample.accel_x_g = packet.accel_x_g;
    sample.accel_y_g = packet.accel_y_g;
    sample.accel_z_g = packet.accel_z_g;
    sample.gyro_x_dps = packet.gyro_x_dps;
    sample.gyro_y_dps = packet.gyro_y_dps;
    sample.gyro_z_dps = packet.gyro_z_dps;

    sample.altitude_m = pressureToAltitudeM(sample.pressure_pa);
    if (!altitude_baseline_initialized_) {
        altitude_baseline_m_ = sample.altitude_m;
        altitude_baseline_initialized_ = true;
    }

    sample.rel_altitude_m = sample.altitude_m - altitude_baseline_m_;
    sample.accel_mag_g = sqrtf((sample.accel_x_g * sample.accel_x_g) +
                               (sample.accel_y_g * sample.accel_y_g) +
                               (sample.accel_z_g * sample.accel_z_g));

    last_status_ = HILReadStatus::SampleReady;
    return true;
}

const char* HILSensorSource::name() const {
    return "HIL";
}

HILReadStatus HILSensorSource::lastStatus() const {
    return last_status_;
}

float HILSensorSource::pressureToAltitudeM(float pressure_pa) const {
    if (pressure_pa <= 0.0f) {
        return 0.0f;
    }

    return 44330.0f * (1.0f - powf(pressure_pa / kSeaLevelPressurePa, kAltitudeExponent));
}

}  // namespace arfc
