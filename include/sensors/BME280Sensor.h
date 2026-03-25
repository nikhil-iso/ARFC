#pragma once

#include <Arduino.h>

namespace arfc {

class BME280Sensor {
   public:
    bool begin();
    bool readPressurePa(float& pressure_pa) const;
    bool readTemperatureC(float& temperature_c) const;
};

}  // namespace arfc

