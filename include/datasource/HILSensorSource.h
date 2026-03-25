#pragma once

#include "datasource/IDataSource.h"
#include "datasource/SerialPacketParser.h"

namespace arfc {

class HILSensorSource : public IDataSource {
   public:
    explicit HILSensorSource(Stream& serial_port);

    bool begin() override;
    bool readSample(FlightSample& sample) override;
    const char* name() const override;

   private:
    static constexpr size_t kBufferSize = 128;

    Stream& serial_port_;
    SerialPacketParser parser_;
    char buffer_[kBufferSize] = {};
    size_t buffer_index_;
};

}  // namespace arfc
