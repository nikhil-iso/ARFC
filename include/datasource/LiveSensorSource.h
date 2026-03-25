#pragma once

#include "datasource/IDataSource.h"
#include "sensors/BME280Sensor.h"
#include "sensors/MPU6050Sensor.h"

namespace arfc {

class LiveSensorSource : public IDataSource {
   public:
    LiveSensorSource();

    bool begin() override;
    bool readSample(FlightSample& sample) override;
    const char* name() const override;

   private:
    BME280Sensor bme280_;
    MPU6050Sensor mpu6050_;
};

}  // namespace arfc

