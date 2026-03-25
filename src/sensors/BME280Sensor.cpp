#include "sensors/BME280Sensor.h"

namespace arfc {

bool BME280Sensor::begin() {
    return true;
}

bool BME280Sensor::readPressurePa(float& pressure_pa) const {
    pressure_pa = 101325.0f;
    return true;
}

bool BME280Sensor::readTemperatureC(float& temperature_c) const {
    temperature_c = 20.0f;
    return true;
}

}  // namespace arfc

