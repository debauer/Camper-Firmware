//
// Created by debauer on 22.12.21.
//

#include "status.h"

// SETTER

void System_Status::setNetworkState(Network_State s){
    network_state = s;
}

void System_Status::setHeaterState(Heater_State s){
    heater_state = s;
}


void System_Status::setMainState(Main_State s){
    main_state = s;
}

// GETTER

Heater_State System_Status::getHeaterState(){
    return heater_state;
}

Network_State System_Status::getNetworkState(){
    return network_state;
}

Main_State System_Status::getMainState(){
    return main_state;
}

void System_Status::print(){
    Serial.print("STATUS: ");
    Serial.print(main_state);
    Serial.print(" ");
    Serial.print(heater_state);
    Serial.print(" ");
    Serial.print(network_state);
    Serial.print(" ");
    Serial.print(rssi);
    Serial.print("db ");
    Serial.println();
}