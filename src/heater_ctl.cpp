//
// Created by debauer on 20.12.21.
//

#include "heater_ctl.h"

Heater_Ctl::Heater_Ctl() {
    target_temperature = DETAULT_TEMPERATURE;
    state = OFF;
    output_pin = -1;
    action = nothing;
    temperature_control = false;

}

void Heater_Ctl::begin(char pin) {
    output_pin = pin;
    pinMode(pin, OUTPUT);
}

void Heater_Ctl::off() {
    action = turn_off;
}

void Heater_Ctl::on() {
    action = nothing;
}

void Heater_Ctl::loop() {
    static State cached_state = INITIAL_STATE;
    if (state != cached_state){
        Serial.println("HEATER_CTL: heater state has changed");
        Serial.print("HEATER_CTL: old - ");
        Serial.println(cached_state);
        Serial.print("HEATER_CTL: new - ");
        Serial.println(state);
        cached_state = state;
    }
    // state maschine override
    if(action == turn_off){
        state = OFF;
        action = nothing;
    }

    switch(state){
        case ON_T_HIGH:
            if(temperature - TEMPERATURE_HISTERESIS < DETAULT_TEMPERATURE){
                state = ON_T_OK;
            }

            break;

        case ON_T_OK:
            if(temperature > DETAULT_TEMPERATURE){
                state = ON_T_HIGH;
            }
            break;

        case OFF:
        default:
            if(action == turn_on){
                if(temperature_control){
                    // to make sure the relai stays off if the temperature is allready to high
                    state = ON_T_HIGH;
                }else{
                    state = ON_PERMANENTLY;
                }
                action = nothing;
            }
            break;
    }

    switch(state){
        case ON_T_OK:
        case ON_PERMANENTLY:
            digitalWrite(output_pin, HIGH);
            break;

        default:
        case OFF:
        case ON_T_HIGH:
            digitalWrite(output_pin, LOW);
            break;
    }
}