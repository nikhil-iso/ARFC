#pragma once

#include <Arduino.h>

namespace arfc {

struct HILPacket {
    uint32_t timestamp_ms = 0;
    float pressure_pa = 0.0f;
    float temperature_c = 0.0f;
    float accel_x_g = 0.0f;
    float accel_y_g = 0.0f;
    float accel_z_g = 0.0f;
    float gyro_x_dps = 0.0f;
    float gyro_y_dps = 0.0f;
    float gyro_z_dps = 0.0f;
};

enum class SerialPacketStatus : uint8_t {
    NoPacket,
    PacketReady,
    InvalidPacket
};

class SerialPacketParser {
   public:
    SerialPacketParser();

    SerialPacketStatus readPacket(Stream& serial_port, HILPacket& packet);

   private:
    static constexpr size_t kBufferSize = 128;
    static constexpr size_t kExpectedFieldCount = 9;

    bool parseLine(char* line, HILPacket& packet) const;
    void resetBuffer();

    char buffer_[kBufferSize] = {};
    size_t buffer_index_;
    bool overflowed_;
};

}  // namespace arfc
