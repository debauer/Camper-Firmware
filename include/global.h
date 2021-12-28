//
// Created by debauer on 20.12.21.
//
#ifndef SIM808_GLOBAL_H
#define SIM808_GLOBAL_H

#include "defines.h"
#include "Adafruit_FONA.h"
#include "LiquidCrystal_I2C.h"
#include "cppQueue.h"
#include "../lib/sim808/sim808.h"

#include "../lib/pin_ctl/pin_ctl.h"

#ifdef UNIT_TEST
#include "debug/fona_dummy.h"
#endif

#define DEBUG
#define SIM808_DUMMY


#define EXTERN

#ifndef GLOBAL_INIT
    #undef EXTERN
    #define EXTERN extern
#endif

// VARIABLES

EXTERN char temperature;

// OBJECTS

#ifdef GLOBAL_INIT
    #ifdef UNIT_TEST
        fona_dummy fona = fona_dummy();
    #else
        Adafruit_FONA fona = Adafruit_FONA(SIM808_RESET);
    #endif
    cppQueue SMS_Queue = cppQueue(sizeof(SMS), SMS_QUEUE_SIZE, FIFO);
    pin_ctl heater = pin_ctl();
    SIM808 sim808 = SIM808();
    LiquidCrystal_I2C lcd (0x27, 16,2);
#else
    #ifdef UNIT_TEST
        extern fona_dummy sim808;
    #else
        extern Adafruit_FONA fona;
    #endif
    extern cppQueue SMS_Queue;
    extern pin_ctl heater;
    extern LiquidCrystal_I2C lcd;
    extern SIM808 sim808;
#endif

#endif //SIM808_GLOBAL_H

#include "../lib/sim808/sim808.h"