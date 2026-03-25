#include "datasource/LiveSensorSource.h"

#include <math.h>

namespace arfc {

LiveSensorSource::LiveSensorSource() = default;

bool LiveSensorSource::begin() {
    const bool bme_ok = bme280_.begin();
    const bool imu_ok = mpu6050_.begin();
    return bme_ok && imu_ok;
}

bool LiveSensorSource::readSample(FlightSample& sample) {
    sample.timestamp_ms = millis();

    if (!bme280_.readPressurePa(sample.pressure_pa)) {
        return false;
    }

    if (!bme280_.readTemperatureC(sample.temperature_c)) {
        return false;
    }

    if (!mpu6050_.readMotion(sample.accel_x_g,
                             sample.accel_y_g,
                             sample.accel_z_g,
                             sample.gyro_x_dps,
                             sample.gyro_y_dps,
                             sample.gyro_z_dps)) {
        return false;
    }

    sample.accel_mag_g = sqrtf((sample.accel_x_g * sample.accel_x_g) +
                               (sample.accel_y_g * sample.accel_y_g) +
                               (sample.accel_z_g * sample.accel_z_g));

    return true;
}

const char* LiveSensorSource::name() const {
    return "LIVE";
}

}  // namespace arfc

