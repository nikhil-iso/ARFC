#include "logging/Logger.h"

#include <stdio.h>
#include <string.h>

namespace arfc {

namespace {

constexpr uint8_t kFloatDigits = 3;

}  // namespace

Logger::Logger()
    : ready_(false),
      chip_select_pin_(AppConfig::kSdChipSelectPin),
      file_name_{0},
      last_flush_ms_(0) {}

Logger::~Logger() {
    if (log_file_) {
        log_file_.flush();
        log_file_.close();
    }
}

bool Logger::begin() {
    ready_ = false;

    if (log_file_) {
        log_file_.close();
    }

    if (!SD.begin(chip_select_pin_)) {
        return false;
    }

    if (!selectLogFileName()) {
        return false;
    }

    log_file_ = SD.open(file_name_, FILE_WRITE);
    if (!log_file_) {
        return false;
    }

    if (!writeHeaderRow()) {
        log_file_.close();
        return false;
    }

    log_file_.flush();
    last_flush_ms_ = millis();
    ready_ = true;
    return true;
}

bool Logger::logSample(const FlightSample& sample) {
    if (!ready_ || !log_file_) {
        return false;
    }

    bool ok = true;

    ok = ok && (log_file_.print(sample.timestamp_ms) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.pressure_pa, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.temperature_c, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.altitude_m, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.rel_altitude_m, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.accel_x_g, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.accel_y_g, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.accel_z_g, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.gyro_x_dps, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.gyro_y_dps, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.gyro_z_dps, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.print(sample.accel_mag_g, kFloatDigits) > 0U);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.launch_flag);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.apogee_flag);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.descent_flag);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.landing_flag);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.continuity_ch1);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.continuity_ch2);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.apogee_cmd);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && writeBoolField(sample.main_cmd);
    ok = ok && (log_file_.print(',') > 0U);
    ok = ok && (log_file_.println(sample.error_flags) > 0U);

    if (!ok) {
        ready_ = false;
        log_file_.close();
        return false;
    }

    flushIfNeeded(millis());
    return true;
}

bool Logger::isReady() const {
    return ready_;
}

const char* Logger::fileName() const {
    return file_name_;
}

bool Logger::selectLogFileName() {
    for (uint8_t index = 0; index < 100; ++index) {
        char candidate[kFileNameSize];
        snprintf(candidate, sizeof(candidate), "flight%02u.csv", index);

        if (!SD.exists(candidate)) {
            strncpy(file_name_, candidate, sizeof(file_name_) - 1U);
            file_name_[sizeof(file_name_) - 1U] = '\0';
            return true;
        }
    }

    return false;
}

bool Logger::writeHeaderRow() {
    // Fixed CSV column order for all log rows.
    return log_file_.println(
               F("timestamp_ms,pressure_pa,temperature_c,altitude_m,rel_altitude_m,"
                 "accel_x_g,accel_y_g,accel_z_g,gyro_x_dps,gyro_y_dps,gyro_z_dps,"
                 "accel_mag_g,launch_flag,apogee_flag,descent_flag,landing_flag,"
                 "continuity_ch1,continuity_ch2,apogee_cmd,main_cmd,error_flags")) > 0U;
}

void Logger::flushIfNeeded(uint32_t now_ms) {
    if ((now_ms - last_flush_ms_) < AppConfig::kLoggerFlushPeriodMs) {
        return;
    }

    log_file_.flush();
    last_flush_ms_ = now_ms;
}

bool Logger::writeBoolField(bool value) {
    return log_file_.print(value ? 1 : 0) > 0U;
}

}  // namespace arfc
