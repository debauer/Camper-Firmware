//
// Created by debauer on 20.12.21.
//

#include "fona_dummy.h"

bool fona_dummy::begin(Stream &port) {
    return true;
}

bool fona_dummy::sendSMS(char *smsaddr, char *smsmsg){ return true;}

int fona_dummy::available() { return true; }

int fona_dummy::read() {
    return fonaSerial->read();
}

uint8_t fona_dummy::getIMEI(char *imei){
    return 2;
}

bool fona_dummy::getSMSSender(uint8_t message_index, char *sender, int senderlen) {
    return true;
}

bool fona_dummy::readSMS(uint8_t message_index, char *smsbuff, uint16_t maxlen, uint16_t *readlen) {
    return true;
}

uint8_t fona_dummy::getRSSI(){
    return 33;
}

uint8_t fona_dummy::getNetworkStatus(){
    return NETWORK_STATUS_REGISTERED;
}
bool fona_dummy::sendCheckReply(FONAFlashStringPtr send, FONAFlashStringPtr reply, uint16_t timeout){
    return true;
}

bool fona_dummy::deleteSMS(uint8_t message_index){
    return true;
}