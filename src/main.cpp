#define GLOBAL_INIT
#include "global.h"
#undef GLOBAL_INIT

void setup() {
    while (!Serial);
    Serial.begin(115200);
    sim808_begin(4800);
    heater.begin(HEATER_RELAY_PIN);
}

void loop() {
    sim808_loop();
    heater.loop();
}