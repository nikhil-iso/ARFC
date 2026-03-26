#pragma once

#include <Arduino.h>

namespace arfc {

struct FlightSample {
    uint32_t timestamp_ms = 0;

    float pressure_pa = 0.0f;
    float temperature_c = 0.0f;
    float altitude_m = 0.0f;
    float rel_altitude_m = 0.0f;

    float accel_x_g = 0.0f;
    float accel_y_g = 0.0f;
    float accel_z_g = 0.0f;

    float gyro_x_dps = 0.0f;
    float gyro_y_dps = 0.0f;
    float gyro_z_dps = 0.0f;

    float accel_mag_g = 0.0f;

    bool launch_flag = false;
    bool apogee_flag = false;
    bool descent_flag = false;
    bool landing_flag = false;

    bool continuity_ch1 = false;
    bool continuity_ch2 = false;
    bool apogee_cmd = false;
    bool main_cmd = false;
    uint32_t error_flags = 0;
};

}  // namespace arfc
