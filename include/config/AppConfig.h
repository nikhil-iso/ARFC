#pragma once

#include <Arduino.h>

namespace arfc {

enum class DataSourceMode : uint8_t {
    MODE_LIVE,
    MODE_HIL
};

struct AppConfig {
    static constexpr DataSourceMode kDataSourceMode = DataSourceMode::MODE_HIL;
    static constexpr uint32_t kLoopPeriodMs = 20;
    static constexpr uint32_t kHeartbeatPeriodMs = 500;
    static constexpr uint32_t kLogPeriodMs = 1000;
    static constexpr uint32_t kLoggerFlushPeriodMs = 1000;
    static constexpr float kLaunchAccelThresholdG = 2.0f;
    static constexpr uint8_t kLaunchDetectSamples = 3;
    static constexpr float kBoostAccelThresholdG = 1.5f;
    static constexpr float kCoastAccelThresholdG = 1.25f;
    static constexpr float kApogeeMinRelAltitudeM = 5.0f;
    static constexpr float kApogeeDeclineDeltaM = 0.25f;
    static constexpr uint8_t kApogeeDeclineSamples = 3;
    static constexpr float kDescentDetectDeltaM = 0.25f;
    static constexpr float kLandingAltitudeDeltaM = 0.15f;
    static constexpr float kLandingAccelToleranceG = 0.15f;
    static constexpr uint8_t kLandingDetectSamples = 10;
    static constexpr uint8_t kLedPin = LED_BUILTIN;
    static constexpr uint8_t kBuzzerPin = 6;

#ifdef BUILTIN_SDCARD
    static constexpr uint8_t kSdChipSelectPin = BUILTIN_SDCARD;
#else
    static constexpr uint8_t kSdChipSelectPin = 10;
#endif
};

}  // namespace arfc
