//
// Created by debauer on 20.12.21.
//

#ifndef SIM808_CTL_H
#define SIM808_CTL_H

class pin_ctl;

#include "Arduino.h"

#define OFF 0
#define ON 1

class pin_ctl {
    protected:
        float thermostat_target;
        char output_pin;
        void thermostat();
        int timer_seconds;
        void timer();
        int state_thermostat;
        int state_timer;
        int state_pin;
        bool active_override;
        bool active_thermostat;
        bool active_timer;
        bool active;
        bool invert_pin;
        void set_pin(bool state) const;
    public:
        pin_ctl();
        void loop();
        void begin(char pin, bool invert = false);
        void on(bool override = false);
        void off(bool override = false);
        void set_timer(int seconds, bool activate);
        void set_thermostat(float target, bool activate);
};


#endif //SIM808_CTL_H
