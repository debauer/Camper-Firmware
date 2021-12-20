//
// Created by debauer on 20.12.21.
//
#ifndef SIM808_GLOBAL_H
#define SIM808_GLOBAL_H

#include "Arduino.h"
#include "Adafruit_FONA.h"
#include "heater_ctl.h"
#include "cppQueue.h"
#include "pinout.h"

#define SMS_QUEUE_SIZE 20
#define SMS_LENGHT 250
#define SMS_PHONE_NUMBER_LENGHT 32

typedef struct sms_type {
	char sender[SMS_PHONE_NUMBER_LENGHT];
	char msg[SMS_LENGHT];
} SMS;

class Heater_Ctl;

#ifdef GLOBAL_INIT
HardwareSerial *fonaSerial = &Serial2;
Adafruit_FONA sim808 = Adafruit_FONA(SIM808_RESET);
cppQueue SMS_Queue = cppQueue(sizeof(SMS), SMS_QUEUE_SIZE, FIFO);
Heater_Ctl heater = Heater_Ctl();
char temperature = 20.0;
#else
extern HardwareSerial *fonaSerial;
extern Adafruit_FONA sim808;
extern cppQueue SMS_Queue;
extern Heater_Ctl heater;
extern char temperature;
#endif

#endif //SIM808_GLOBAL_H

#include "sim808.h"