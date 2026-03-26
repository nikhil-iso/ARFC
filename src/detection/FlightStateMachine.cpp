#include "detection/FlightStateMachine.h"

#include "config/AppConfig.h"

namespace arfc {

FlightStateMachine::FlightStateMachine() : state_(FlightState::BOOT) {}

void FlightStateMachine::begin() {
    transitionTo(FlightState::INIT);
}

void FlightStateMachine::reset() {
    state_ = FlightState::BOOT;
}

FlightState FlightStateMachine::update(const FlightSample& sample, const EventFlags& events) {
    if (sample.error_flags != 0U) {
        transitionTo(FlightState::ERROR);
        return state_;
    }

    switch (state_) {
        case FlightState::BOOT:
            transitionTo(FlightState::INIT);
            break;

        case FlightState::INIT:
            transitionTo(FlightState::PAD_IDLE);
            break;

        case FlightState::PAD_IDLE:
            if (events.launch_flag) {
                transitionTo(FlightState::LAUNCH_DETECTED);
            }
            break;

        case FlightState::LAUNCH_DETECTED:
            if (events.apogee_flag) {
                transitionTo(FlightState::APOGEE_DETECTED);
            } else if (sample.accel_mag_g >= AppConfig::kBoostAccelThresholdG) {
                transitionTo(FlightState::BOOST);
            } else {
                transitionTo(FlightState::COAST);
            }
            break;

        case FlightState::BOOST:
            if (events.apogee_flag) {
                transitionTo(FlightState::APOGEE_DETECTED);
            } else if (sample.accel_mag_g < AppConfig::kCoastAccelThresholdG) {
                transitionTo(FlightState::COAST);
            }
            break;

        case FlightState::COAST:
            if (events.apogee_flag) {
                transitionTo(FlightState::APOGEE_DETECTED);
            }
            break;

        case FlightState::APOGEE_DETECTED:
            if (events.descent_flag) {
                transitionTo(FlightState::DESCENT);
            }
            break;

        case FlightState::DESCENT:
            if (events.landing_flag) {
                transitionTo(FlightState::LANDED);
            }
            break;

        case FlightState::LANDED:
        case FlightState::ERROR:
            break;
    }

    return state_;
}

FlightState FlightStateMachine::state() const {
    return state_;
}

void FlightStateMachine::transitionTo(FlightState new_state) {
    if (state_ == new_state) {
        return;
    }

    state_ = new_state;
    Serial.print("Flight state -> ");
    Serial.println(toString(state_));
}

}  // namespace arfc
