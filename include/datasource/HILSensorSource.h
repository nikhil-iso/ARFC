#pragma once

#include "datasource/IDataSource.h"
#include "datasource/SerialPacketParser.h"

namespace arfc {

enum class HILReadStatus : uint8_t {
    NoData,
    SampleReady,
    InvalidPacket
};

class HILSensorSource : public IDataSource {
   public:
    explicit HILSensorSource(Stream& serial_port);

    bool begin() override;
    bool readSample(FlightSample& sample) override;
    const char* name() const override;
    HILReadStatus lastStatus() const;

   private:
    float pressureToAltitudeM(float pressure_pa) const;

    Stream& serial_port_;
    SerialPacketParser parser_;
    bool altitude_baseline_initialized_;
    float altitude_baseline_m_;
    HILReadStatus last_status_;
};

}  // namespace arfc
