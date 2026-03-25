#include "detection/FlightStateMachine.h"

namespace arfc {

FlightStateMachine::FlightStateMachine() : state_(FlightState::BOOT) {}

void FlightStateMachine::begin() {
    state_ = FlightState::INIT;
}

void FlightStateMachine::reset() {
    state_ = FlightState::BOOT;
}

FlightState FlightStateMachine::update(const FlightSample& sample, const EventFlags& events) {
    (void)sample;
    (void)events;
    return state_;
}

FlightState FlightStateMachine::state() const {
    return state_;
}

}  // namespace arfc

