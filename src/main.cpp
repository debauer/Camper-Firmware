#define GLOBAL_INIT
#include "global.h"
#undef GLOBAL_INIT

#ifndef UNIT_TEST
void setup() {
    while (!Serial);
    Serial.begin(BAUD_SERIAL);
    sim808.begin(BAUD_SIM808);
    heater.begin(HEATER_RELAY_PIN);
}
#endif

int loopcounter = 0;

#ifndef UNIT_TEST

void loop() {
    static bool tested = false;
    sim808.loop();
    heater.loop();
    delay(500);
    system_status.print();
    loopcounter++;
    if(loopcounter >= 10){
        if(!tested){
            sim808.poll_sms();
            tested = true;
        }
        if(system_status.getNetworkState() != REGISTERED){
            sim808.reset();
        }
        loopcounter = 0;
    }
}
#endif