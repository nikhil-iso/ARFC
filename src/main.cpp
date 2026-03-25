#include <Arduino.h>

#include "status/StatusManager.h"

namespace {

arfc::StatusManager g_status_manager;

}  // namespace

void setup() {
    Serial.begin(115200);
    g_status_manager.begin();
    g_status_manager.setFlightState(arfc::FlightState::BOOT);
    Serial.println("ARFC booting...");
}

void loop() {
    g_status_manager.update(millis());
}

