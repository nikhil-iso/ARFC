#pragma once

#include <Arduino.h>

#include "data/FlightSample.h"

namespace arfc {

struct EventFlags {
    bool launch_flag = false;
    bool apogee_flag = false;
    bool descent_flag = false;
    bool landing_flag = false;
};

class EventDetector {
   public:
    EventDetector();

    void reset();
    EventFlags update(const FlightSample& sample);

   private:
    float last_rel_altitude_m_;
    uint8_t launch_counter_;
    uint8_t descending_counter_;
    uint8_t landing_counter_;
    bool initialized_;
    bool launch_detected_;
    bool apogee_detected_;
    bool descent_detected_;
    bool landing_detected_;
};

}  // namespace arfc
