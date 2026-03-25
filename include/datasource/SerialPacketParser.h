#pragma once

#include <Arduino.h>

#include "data/FlightSample.h"

namespace arfc {

class SerialPacketParser {
   public:
    bool parseLine(const char* line, FlightSample& sample) const;
};

}  // namespace arfc

