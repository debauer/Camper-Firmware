//
// Created by debauer on 22.12.21.
//

#ifndef SIM808_STATUS_H
#define SIM808_STATUS_H

class System_Status;

#include "global.h"

class System_Status {
protected:
    Network_State network_state;
    Heater_State heater_state;
    Main_State main_state;


public:
    void setNetworkState(Network_State status);
    void setHeaterState(Heater_State status);
    void setMainState(Main_State status);
    Network_State getNetworkState();
    Heater_State getHeaterState();
    Main_State getMainState();
    void print();

    int rssi;
};


#endif //SIM808_STATUS_H
