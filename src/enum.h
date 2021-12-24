//
// Created by debauer on 22.12.21.
//

#ifndef SIM808_ENUM_H
#define SIM808_ENUM_H

enum SMS_Status{
    SMS_UNDEF,
    SMS_REC_UNREAD, //  Received unread messages
    SMS_REC_READ, //  Received read messages
    SMS_STO_UNSENT, //  Stored unsent messages
    SMS_STO_SENT, //  Stored sent messages
    SMS_ALL, //  All messages
};

enum Main_State {
    INIT, CONNECTING, WORKING, ERROR
};

enum Network_State {
    NOT_REGISTERED, REGISTERED, SEARCHING, DENIED, UNKNOWN, ROAMING
};

enum Heater_State {
    HEATER_OFF, HEATER_ON_PERMANENTLY, ON_T_HIGH, ON_T_OK
};

enum Heater_Action {
    HEATER_DO_NOTHING,
    HEATER_TURN_OFF,
    HEATER_TURN_ON
};

#endif //SIM808_ENUM_H
