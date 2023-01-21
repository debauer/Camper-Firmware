//
// Created by debauer on 24.12.21.
//

#ifndef SIM808_TIMER_H
#define SIM808_TIMER_H


class timer {
public:
    timer();
    bool is_expired();
    void inc();
    void load(int ms, bool reload);
    void disable();
    void enable();

protected:
    int _target;
    int _value;
    bool _reload;
    bool _expired;
    bool _enabled;
};


#endif //SIM808_TIMER_H