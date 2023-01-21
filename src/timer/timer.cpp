//
// Created by debauer on 24.12.21.
//

#include "timer.h"

timer::timer(){
    _reload = false;
    _expired = false;
    _target = 0;
    _value = 0;
    _enabled = false;
}

void timer::enable(){
    _enabled = true;
}

void timer::disable(){
    _enabled = false;
}

bool timer::is_expired() {
    if(_expired && _enabled){
        _expired = false;
        return true;
    }
    return false;
}

void timer::inc() {
    if(_enabled) {
        _value++;
        if (_value >= _target) {
            if (_reload) {
                _value = 0;
            }
            _expired = true;
        }
    }
}

void timer::load(int ms, bool reload) {
    _reload = reload;
    _target = ms;
}
