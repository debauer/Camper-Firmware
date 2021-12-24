//
// Created by debauer on 20.12.21.
//

#include "heater_ctl.h"

Heater_Ctl::Heater_Ctl() {
    target_temperature = HEATER_DETAULT_TEMPERATURE;
    system_status.setHeaterState(HEATER_OFF);
    output_pin = -1;
    action = HEATER_DO_NOTHING;
    temperature_control = false;

}

void Heater_Ctl::begin(char pin) {
    output_pin = pin;
    pinMode(pin, OUTPUT);
}

void Heater_Ctl::off() {
    action = HEATER_TURN_OFF;
}

void Heater_Ctl::on() {
    action = HEATER_DO_NOTHING;
}

void Heater_Ctl::loop() {
    static Heater_State cached_state = HEATER_OFF;
    Heater_State state = system_status.getHeaterState();
    if (state != cached_state){
        Serial.println("HEATER_CTL: heater state has changed");
        Serial.print("HEATER_CTL: old - ");
        Serial.println(cached_state);
        Serial.print("HEATER_CTL: new - ");
        Serial.println(state);
        cached_state = state;
    }
    // state maschine override
    if(action == HEATER_TURN_OFF){
        state = HEATER_OFF;
        action = HEATER_DO_NOTHING;
    }

    switch(state){
        case ON_T_HIGH:
            if(temperature - HEATER_TEMPERATURE_HISTERESIS < HEATER_DETAULT_TEMPERATURE){
                state = ON_T_OK;
            }

            break;

        case ON_T_OK:
            if(temperature > HEATER_DETAULT_TEMPERATURE){
                state = ON_T_HIGH;
            }
            break;

        case HEATER_OFF:
        default:
            if(action == HEATER_TURN_ON){
                if(temperature_control){
                    // to make sure the relai stays off if the temperature is allready to high
                    state = ON_T_HIGH;
                }else{
                    state = HEATER_ON_PERMANENTLY;
                }
                action = HEATER_DO_NOTHING;
            }
            break;
    }

    switch(state){
        case ON_T_OK:
        case HEATER_ON_PERMANENTLY:
            digitalWrite(output_pin, HIGH);
            break;

        default:
        case HEATER_OFF:
        case ON_T_HIGH:
            digitalWrite(output_pin, LOW);
            break;
    }

    system_status.setHeaterState(state);
}