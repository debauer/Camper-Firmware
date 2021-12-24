//
// Created by debauer on 20.12.21.
//

#ifndef SIM808_HEATER_CTL_H
#define SIM808_HEATER_CTL_H

class Heater_Ctl;

#include "global.h"

class Heater_Ctl {
    protected:
        Heater_Action action;
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
