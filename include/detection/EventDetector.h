#pragma once

#include <Arduino.h>

#include "data/FlightSample.h"

namespace arfc {

struct EventFlags {
    bool launch_detected = false;
    bool apogee_detected = false;
    bool landing_detected = false;
};

class EventDetector {
   public:
    void reset();
    EventFlags update(const FlightSample& sample);

   private:
    float last_altitude_m_ = 0.0f;
    bool initialized_ = false;
};

}  // namespace arfc

