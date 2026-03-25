#pragma once

#include <Arduino.h>

namespace arfc {

enum class FlightState : uint8_t {
    BOOT,
    INIT,
    PAD_IDLE,
    LAUNCH_DETECTED,
    BOOST,
    COAST,
    APOGEE_DETECTED,
    DESCENT,
    LANDED,
    ERROR
};

inline const char* toString(FlightState state) {
    switch (state) {
        case FlightState::BOOT:
            return "BOOT";
        case FlightState::INIT:
            return "INIT";
        case FlightState::PAD_IDLE:
            return "PAD_IDLE";
        case FlightState::LAUNCH_DETECTED:
            return "LAUNCH_DETECTED";
        case FlightState::BOOST:
            return "BOOST";
        case FlightState::COAST:
            return "COAST";
        case FlightState::APOGEE_DETECTED:
            return "APOGEE_DETECTED";
        case FlightState::DESCENT:
            return "DESCENT";
        case FlightState::LANDED:
            return "LANDED";
        case FlightState::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

}  // namespace arfc

