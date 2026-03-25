#pragma once

#include <Arduino.h>

#include "config/AppConfig.h"
#include "data/FlightState.h"

namespace arfc {

class StatusManager {
   public:
    StatusManager(uint8_t led_pin = AppConfig::kLedPin,
                  uint8_t buzzer_pin = AppConfig::kBuzzerPin);

    void begin();
    void update(uint32_t now_ms);
    void setFlightState(FlightState state);
    void setError(bool has_error);

   private:
    uint8_t led_pin_;
    uint8_t buzzer_pin_;
    bool led_on_;
    bool has_error_;
    uint32_t last_heartbeat_ms_;
    FlightState flight_state_;
};

}  // namespace arfc

