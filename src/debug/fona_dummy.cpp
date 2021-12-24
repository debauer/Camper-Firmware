//
// Created by debauer on 20.12.21.
//

#include "fona_dummy.h"

bool fona_dummy::begin(Stream &port) {
    fonaSerial->begin(9600);
    return true;
}

bool fona_dummy::sendSMS(char *smsaddr, char *smsmsg){ return true;}

int fona_dummy::available() { return fonaSerial->available(); }

int fona_dummy::read() {
    return fonaSerial->read();
}

uint8_t fona_dummy::getIMEI(char *imei){
    strcpy(imei, "testimei");
    return 8; // imei lenght
}

bool fona_dummy::getSMSSender(uint8_t message_index, char *sender, int senderlen) {
    strcpy(sender, "0123456789");
    return true;
}

bool fona_dummy::readSMS(uint8_t message_index, char *smsbuff, uint16_t maxlen, uint16_t *readlen) {
    strcpy(smsbuff, "test sms");
    *readlen = 8;
    return true;
}

uint8_t fona_dummy::getRSSI(){
    return 15;
}

uint8_t fona_dummy::getNetworkStatus(){
    return 1; // NETWORK_STATUS_REGISTERED
}

bool fona_dummy::sendCheckReply(FONAFlashStringPtr send, FONAFlashStringPtr reply, uint16_t timeout){
    return true;
}

bool fona_dummy::deleteSMS(uint8_t message_index){
    return true;
}