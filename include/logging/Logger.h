#pragma once

#include <Arduino.h>
#include <SD.h>

#include "config/AppConfig.h"
#include "data/FlightSample.h"

namespace arfc {

class Logger {
   public:
    Logger();
    ~Logger();

    bool begin();
    bool logSample(const FlightSample& sample);
    bool isReady() const;
    const char* fileName() const;

   private:
    static constexpr size_t kFileNameSize = 16;

    bool selectLogFileName();
    bool writeHeaderRow();
    void flushIfNeeded(uint32_t now_ms);
    bool writeBoolField(bool value);

    bool ready_;
    uint8_t chip_select_pin_;
    File log_file_;
    char file_name_[kFileNameSize];
    uint32_t last_flush_ms_;
};

}  // namespace arfc
