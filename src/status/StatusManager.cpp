#include "status/StatusManager.h"

namespace arfc {

StatusManager::StatusManager(uint8_t led_pin, uint8_t buzzer_pin)
    : led_pin_(led_pin),
      buzzer_pin_(buzzer_pin),
      led_on_(false),
      has_error_(false),
      last_heartbeat_ms_(0),
      flight_state_(FlightState::BOOT) {}

void StatusManager::begin() {
    pinMode(led_pin_, OUTPUT);
    digitalWrite(led_pin_, LOW);

    pinMode(buzzer_pin_, OUTPUT);
    digitalWrite(buzzer_pin_, LOW);
}

void StatusManager::update(uint32_t now_ms) {
    if ((now_ms - last_heartbeat_ms_) < AppConfig::kHeartbeatPeriodMs) {
        return;
    }

    last_heartbeat_ms_ = now_ms;
    led_on_ = !led_on_;
    digitalWrite(led_pin_, led_on_ ? HIGH : LOW);

    if (has_error_) {
        digitalWrite(buzzer_pin_, led_on_ ? HIGH : LOW);
    } else {
        digitalWrite(buzzer_pin_, LOW);
    }
}

void StatusManager::setFlightState(FlightState state) {
    flight_state_ = state;
    (void)flight_state_;
}

void StatusManager::setError(bool has_error) {
    has_error_ = has_error;
}

}  // namespace arfc
