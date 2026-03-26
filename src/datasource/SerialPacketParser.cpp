#include "datasource/SerialPacketParser.h"

#include <math.h>
#include <stdlib.h>

namespace arfc {

namespace {

bool parseUnsignedLong(char* token, uint32_t& value) {
    if (token == nullptr || *token == '\0') {
        return false;
    }

    if (*token == '-') {
        return false;
    }

    char* end_ptr = nullptr;
    const unsigned long parsed = strtoul(token, &end_ptr, 10);
    if (end_ptr == token || *end_ptr != '\0') {
        return false;
    }

    value = static_cast<uint32_t>(parsed);
    return true;
}

bool parseFloat(char* token, float& value) {
    if (token == nullptr || *token == '\0') {
        return false;
    }

    char* end_ptr = nullptr;
    value = strtof(token, &end_ptr);
    return end_ptr != token && *end_ptr == '\0' && isfinite(value);
}

}  // namespace

SerialPacketParser::SerialPacketParser() : buffer_index_(0), overflowed_(false) {}

SerialPacketStatus SerialPacketParser::readPacket(Stream& serial_port, HILPacket& packet) {
    while (serial_port.available() > 0) {
        const int incoming = serial_port.read();
        if (incoming < 0) {
            continue;
        }

        if (incoming == '\r') {
            continue;
        }

        if (incoming == '\n') {
            if (overflowed_ || buffer_index_ == 0U) {
                resetBuffer();
                return SerialPacketStatus::InvalidPacket;
            }

            buffer_[buffer_index_] = '\0';
            const bool parsed_ok = parseLine(buffer_, packet);
            resetBuffer();
            return parsed_ok ? SerialPacketStatus::PacketReady
                             : SerialPacketStatus::InvalidPacket;
        }

        if (overflowed_) {
            continue;
        }

        if (buffer_index_ >= (kBufferSize - 1U)) {
            overflowed_ = true;
            continue;
        }

        buffer_[buffer_index_++] = static_cast<char>(incoming);
    }

    return SerialPacketStatus::NoPacket;
}

bool SerialPacketParser::parseLine(char* line, HILPacket& packet) const {
    if (line == nullptr || *line == '\0') {
        return false;
    }

    char* fields[kExpectedFieldCount] = {};
    size_t field_count = 0;
    fields[field_count++] = line;

    for (char* cursor = line; *cursor != '\0'; ++cursor) {
        if (*cursor != ',') {
            continue;
        }

        *cursor = '\0';
        if (field_count >= kExpectedFieldCount) {
            return false;
        }

        fields[field_count++] = cursor + 1;
    }

    if (field_count != kExpectedFieldCount) {
        return false;
    }

    for (size_t i = 0; i < field_count; ++i) {
        if (fields[i] == nullptr || *fields[i] == '\0') {
            return false;
        }
    }

    return parseUnsignedLong(fields[0], packet.timestamp_ms) &&
           parseFloat(fields[1], packet.pressure_pa) &&
           parseFloat(fields[2], packet.temperature_c) &&
           parseFloat(fields[3], packet.accel_x_g) &&
           parseFloat(fields[4], packet.accel_y_g) &&
           parseFloat(fields[5], packet.accel_z_g) &&
           parseFloat(fields[6], packet.gyro_x_dps) &&
           parseFloat(fields[7], packet.gyro_y_dps) &&
           parseFloat(fields[8], packet.gyro_z_dps);
}

void SerialPacketParser::resetBuffer() {
    buffer_index_ = 0;
    overflowed_ = false;
    buffer_[0] = '\0';
}

}  // namespace arfc
