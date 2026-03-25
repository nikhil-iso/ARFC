#include "detection/EventDetector.h"

namespace arfc {

void EventDetector::reset() {
    last_altitude_m_ = 0.0f;
    initialized_ = false;
}

EventFlags EventDetector::update(const FlightSample& sample) {
    EventFlags events;

    if (!initialized_) {
        last_altitude_m_ = sample.altitude_m;
        initialized_ = true;
        return events;
    }

    last_altitude_m_ = sample.altitude_m;
    return events;
}

}  // namespace arfc

