#pragma once

#include <Arduino.h>

#include "data/FlightSample.h"

namespace arfc {

class IDataSource {
   public:
    virtual ~IDataSource() = default;

    virtual bool begin() = 0;
    virtual bool readSample(FlightSample& sample) = 0;
    virtual const char* name() const = 0;
};

}  // namespace arfc

