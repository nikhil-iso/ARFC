#include "sensors/MPU6050Sensor.h"

namespace arfc {

bool MPU6050Sensor::begin() {
    return true;
}

bool MPU6050Sensor::readMotion(float& accel_x_g,
                               float& accel_y_g,
                               float& accel_z_g,
                               float& gyro_x_dps,
                               float& gyro_y_dps,
                               float& gyro_z_dps) const {
    accel_x_g = 0.0f;
    accel_y_g = 0.0f;
    accel_z_g = 1.0f;

    gyro_x_dps = 0.0f;
    gyro_y_dps = 0.0f;
    gyro_z_dps = 0.0f;

    return true;
}

}  // namespace arfc

