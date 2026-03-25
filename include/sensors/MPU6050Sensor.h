#pragma once

#include <Arduino.h>

namespace arfc {

class MPU6050Sensor {
   public:
    bool begin();
    bool readMotion(float& accel_x_g,
                    float& accel_y_g,
                    float& accel_z_g,
                    float& gyro_x_dps,
                    float& gyro_y_dps,
                    float& gyro_z_dps) const;
};

}  // namespace arfc

