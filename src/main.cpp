#define GLOBAL_INIT
#include "global.h"
#undef GLOBAL_INIT

int loopcounter = 0;
hw_timer_t * hw_timer0 = NULL;
hw_timer_t * hw_timer1 = NULL;

void IRAM_ATTR sec_timer() {
    // every 1ms
}

int t;
void IRAM_ATTR ms_timer() {
    // every 1ms
    t++;
}

#ifndef UNIT_TEST
void setup() {
    while (!Serial);
    Serial.begin(BAUD_SERIAL);
    sim808.begin(&fona, &Serial2, BAUD_SIM808, SIM808_PWRKEY, SIM808_STATUS, SIM808_RESET, SIM808_RESET_RELAY);
    heater.begin(HEATER_RELAY_PIN);
    lcd.init();

  // Turn on the backlight on LCD.
    lcd.backlight();

  // print the Message on the LCD.
    lcd.print( "init" );
    hw_timer0 = timerBegin(0, 80, true);
    hw_timer1 = timerBegin(1, 80, true);
    timerAttachInterrupt(hw_timer0, &sec_timer, true);
    timerAttachInterrupt(hw_timer1, &ms_timer, true);
    timerAlarmWrite(hw_timer0, 1000000, true); // 1000000 == 1s
    timerAlarmWrite(hw_timer1, 1000, true); // 1000 == 1ms
    timerAlarmEnable(hw_timer0);
    timerAlarmEnable(hw_timer1);
}
#endif

#ifndef UNIT_TEST
void loop() {
    static bool tested = false;
    sim808.loop();
    heater.loop();
    delay(500);
    if(t >= 5000){
        t=0;
        if(!tested){
            sim808.poll_sms();
            tested = true;
        }
        if(sim808.get_network_state() != REGISTERED){
            sim808.reset();
        }
    }
}
#endif

