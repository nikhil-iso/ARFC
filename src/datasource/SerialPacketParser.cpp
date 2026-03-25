#include "datasource/SerialPacketParser.h"

#include <stdlib.h>
#include <string.h>

namespace arfc {

namespace {

bool parseUnsignedLong(char* token, uint32_t& value) {
    if (token == nullptr || *token == '\0') {
        return false;
    }

    char* end_ptr = nullptr;
    const unsigned long parsed = strtoul(token, &end_ptr, 10);
    if (end_ptr == token || *end_ptr != '\0') {
        return false;
    }

    value = static_cast<uint32_t>(parsed);
    return true;
}

bool parseFloat(char* token, float& value) {
    if (token == nullptr || *token == '\0') {
        return false;
    }

    char* end_ptr = nullptr;
    value = strtof(token, &end_ptr);
    return end_ptr != token && *end_ptr == '\0';
}

}  // namespace

bool SerialPacketParser::parseLine(const char* line, FlightSample& sample) const {
    if (line == nullptr || *line == '\0') {
        return false;
    }

    char buffer[128];
    strncpy(buffer, line, sizeof(buffer) - 1U);
    buffer[sizeof(buffer) - 1U] = '\0';

    char* token = strtok(buffer, ",");

    if (!parseUnsignedLong(token, sample.timestamp_ms)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.pressure_pa)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.temperature_c)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.accel_x_g)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.accel_y_g)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.accel_z_g)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.gyro_x_dps)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.gyro_y_dps)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (!parseFloat(token, sample.gyro_z_dps)) {
        return false;
    }

    token = strtok(nullptr, ",");
    if (token != nullptr) {
        return false;
    }

    sample.altitude_m = 0.0f;
    sample.rel_altitude_m = 0.0f;
    return true;
}

}  // namespace arfc
