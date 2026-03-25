#pragma once

#include <Arduino.h>

#include "data/FlightSample.h"
#include "data/FlightState.h"

namespace arfc {

class Logger {
   public:
    Logger();

    bool begin();
    bool logSample(const FlightSample& sample, FlightState state);
    bool isReady() const;

   private:
    bool ready_;
};

}  // namespace arfc

