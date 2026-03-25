#include "logging/Logger.h"

namespace arfc {

Logger::Logger() : ready_(false) {}

bool Logger::begin() {
    ready_ = true;
    return ready_;
}

bool Logger::logSample(const FlightSample& sample, FlightState state) {
    (void)sample;
    (void)state;
    return ready_;
}

bool Logger::isReady() const {
    return ready_;
}

}  // namespace arfc

