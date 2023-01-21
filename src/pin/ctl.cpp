//
// Created by debauer on 20.12.21.
//

#include "ctl.h"


pin_ctl::pin_ctl() {
    output_pin = -1;
    state_thermostat = OFF;
    state_pin = OFF;
    invert_pin = false;
}

void pin_ctl::begin(char pin, bool invert) {
    output_pin = pin;
    invert_pin = invert;
    pinMode(pin, OUTPUT);
}

void pin_ctl::off(bool override) {
    active = OFF;
    active_override = override;
}

void pin_ctl::on(bool override) {
    active = ON;
    active_override = override;
}

void pin_ctl::set_timer(int seconds, bool activate){
    timer_seconds = seconds;
    active_timer = activate;
}

void pin_ctl::set_thermostat(float target, bool activate ){
    thermostat_target = target;
    active_thermostat = activate;
}

void pin_ctl::thermostat(){
    if(active_thermostat){
        state_thermostat = ON;
    }else{
        state_thermostat = OFF;
    }
}

void pin_ctl::timer(){
    if(active_timer){
        state_timer = ON;
    }else{
        state_timer = OFF;
    }
}

void pin_ctl::set_pin(bool state) const{
    digitalWrite(output_pin, invert_pin ^ state);
}

void pin_ctl::loop() {
    thermostat();
    timer();
    if(active_override){
        set_pin(active); // active is ON or OFF and if override, this is also the pin state
        return;
    }
    set_pin(state_thermostat && state_timer);
}