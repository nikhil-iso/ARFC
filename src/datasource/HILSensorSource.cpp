#include "datasource/HILSensorSource.h"

#include <math.h>

namespace arfc {

HILSensorSource::HILSensorSource(Stream& serial_port)
    : serial_port_(serial_port), buffer_index_(0) {}

bool HILSensorSource::begin() {
    buffer_index_ = 0;
    return true;
}

bool HILSensorSource::readSample(FlightSample& sample) {
    while (serial_port_.available() > 0) {
        const int incoming = serial_port_.read();
        if (incoming < 0) {
            continue;
        }

        if (incoming == '\r') {
            continue;
        }

        if (incoming == '\n') {
            buffer_[buffer_index_] = '\0';
            buffer_index_ = 0;

            if (!parser_.parseLine(buffer_, sample)) {
                return false;
            }

            sample.accel_mag_g = sqrtf((sample.accel_x_g * sample.accel_x_g) +
                                       (sample.accel_y_g * sample.accel_y_g) +
                                       (sample.accel_z_g * sample.accel_z_g));
            return true;
        }

        if (buffer_index_ >= (kBufferSize - 1U)) {
            buffer_index_ = 0;
            return false;
        }

        buffer_[buffer_index_++] = static_cast<char>(incoming);
    }

    return false;
}

const char* HILSensorSource::name() const {
    return "HIL";
}

}  // namespace arfc
