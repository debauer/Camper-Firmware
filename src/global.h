//
// Created by debauer on 20.12.21.
//
#ifndef SIM808_GLOBAL_H
#define SIM808_GLOBAL_H

#include "Arduino.h"
#include "defines.h"
#include "enum.h"
#include "struct.h" // after enum!
#include "Adafruit_FONA.h"
#include "heater_ctl.h"
#include "cppQueue.h"
#include "status.h"
#ifdef UNIT_TEST
#include "debug/fona_dummy.h"
#endif

#define DEBUG
#define SIM808_DUMMY

#define SMS_QUEUE_SIZE 20
#define SMS_LENGHT 250
#define SMS_PHONE_NUMBER_LENGHT 32


class Heater_Ctl;

#ifdef GLOBAL_INIT
    HardwareSerial *fonaSerial = &Serial2;
    #ifdef UNIT_TEST
        fona_dummy fona = fona_dummy();
    #else
        Adafruit_FONA fona = Adafruit_FONA(SIM808_RESET);
    #endif
        System_Status system_status = System_Status();
    cppQueue SMS_Queue = cppQueue(sizeof(SMS), SMS_QUEUE_SIZE, FIFO);
    Heater_Ctl heater = Heater_Ctl();
    SIM808 sim808 = SIM808();
    char temperature = 20.0;
#else
    extern HardwareSerial *fonaSerial;
    #ifdef UNIT_TEST
        extern fona_dummy sim808;
    #else
        extern Adafruit_FONA fona;
    #endif
    extern cppQueue SMS_Queue;
    extern Heater_Ctl heater;
    extern char temperature;
    extern System_Status system_status;
    extern SIM808 sim808;
#endif

#endif //SIM808_GLOBAL_H

#include "sim808.h"