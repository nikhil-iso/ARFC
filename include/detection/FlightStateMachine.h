#pragma once

#include <Arduino.h>

#include "data/FlightSample.h"
#include "data/FlightState.h"
#include "detection/EventDetector.h"

namespace arfc {

class FlightStateMachine {
   public:
    FlightStateMachine();

    void begin();
    void reset();
    FlightState update(const FlightSample& sample, const EventFlags& events);
    FlightState state() const;

   private:
    void transitionTo(FlightState new_state);

    FlightState state_;
};

}  // namespace arfc
