#define GLOBAL_INIT
#include "global.h"
#undef GLOBAL_INIT

hw_timer_t * hw_timer0 = nullptr;
hw_timer_t * hw_timer1 = nullptr;
timer sim_reset_timer = timer();
timer sim808_loop_timer = timer();
timer heater_loop_timer = timer();
timer sms_poll_timer = timer();

void IRAM_ATTR sec_timer() {
    // every 1ms
}

int t;
void IRAM_ATTR ms_timer() {
    // every 1ms
    sim_reset_timer.inc();
    sim808_loop_timer.inc();
    heater_loop_timer.inc();
    sms_poll_timer.inc();
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

    sim808_loop_timer.load(500, true);
    sim808_loop_timer.enable();

    heater_loop_timer.load(500, true);
    heater_loop_timer.enable();

    sms_poll_timer.load(5000, false);
    sms_poll_timer.enable();

    sim_reset_timer.load(5000, true);
    sim_reset_timer.enable();

    hw_timer0 = timerBegin(0, 80, true);
    timerAttachInterrupt(hw_timer0, &sec_timer, true);
    timerAlarmWrite(hw_timer0, TIMER_1S, true);
    timerAlarmEnable(hw_timer0);

    hw_timer1 = timerBegin(1, 80, true);
    timerAttachInterrupt(hw_timer1, &ms_timer, true);
    timerAlarmWrite(hw_timer1, TIMER_1MS, true);
    timerAlarmEnable(hw_timer1);
}
#endif

#ifndef UNIT_TEST
void loop() {
    if(sim808_loop_timer.is_expired()) sim808.loop();
    if(heater_loop_timer.is_expired()) heater.loop();
    if(sms_poll_timer.is_expired()) sim808.poll_sms();
    if(sim_reset_timer.is_expired()){
        if(sim808.get_network_state() != REGISTERED){
            sim808.reset();
        }
    }
}
#endif

