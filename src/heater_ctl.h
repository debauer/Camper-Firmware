//
// Created by debauer on 20.12.21.
//

#ifndef SIM808_HEATER_CTL_H
#define SIM808_HEATER_CTL_H

#include "global.h"

#define DETAULT_TEMPERATURE 20.0
#define TEMPERATURE_HISTERESIS 1.0
#define INITIAL_STATE OFF

enum State {
    OFF, ON_PERMANENTLY, ON_T_HIGH, ON_T_OK
};

enum Action {
    nothing,
    turn_off,
    turn_on
};

class Heater_Ctl {
    protected:
        State state;
        Action action;
        float target_temperature;
        bool temperature_control;
        char output_pin;
    public:
        Heater_Ctl();
        void loop();
        void begin(char pin);
        void on();
        void off();
};


#endif //SIM808_HEATER_CTL_H
