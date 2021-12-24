//class Heater_Ctl
// Created by debauer on 20.12.21.
//

#ifndef SIM808_FONA_DUMMY_H
#define SIM808_FONA_DUMMY_H

class fona_dummy;

#include "global.h"

class fona_dummy {
public:
    bool begin(Stream &port);
    bool sendSMS(char *smsaddr, char *smsmsg);
    int available();
    int read();
    bool deleteSMS(uint8_t message_index);
    bool sendCheckReply(FONAFlashStringPtr send, FONAFlashStringPtr reply, uint16_t timeout);
    uint8_t getNetworkStatus();
    uint8_t getRSSI();
    bool readSMS(uint8_t message_index, char *smsbuff, uint16_t maxlen, uint16_t *readlen);
    bool getSMSSender(uint8_t message_index, char *sender, int senderlen);
    uint8_t getIMEI(char *imei);
};


#endif //SIM808_FONA_DUMMY_H
