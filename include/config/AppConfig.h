#pragma once

#include <Arduino.h>

namespace arfc {

enum class DataSourceMode : uint8_t {
    Live,
    HIL
};

struct AppConfig {
    static constexpr DataSourceMode kDataSourceMode = DataSourceMode::Live;
    static constexpr uint32_t kLoopPeriodMs = 20;
    static constexpr uint32_t kHeartbeatPeriodMs = 500;
    static constexpr uint8_t kLedPin = LED_BUILTIN;
    static constexpr uint8_t kBuzzerPin = 6;

#ifdef BUILTIN_SDCARD
    static constexpr uint8_t kSdChipSelectPin = BUILTIN_SDCARD;
#else
    static constexpr uint8_t kSdChipSelectPin = 10;
#endif
};

}  // namespace arfc

