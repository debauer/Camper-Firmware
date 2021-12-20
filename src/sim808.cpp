//
// Created by debauer on 16.11.21.
//

#include "sim808.h"

char smsBuffer[250];
char fonaNotificationBuffer[64];

void sim808_power_on(){
    Serial.println(F("PWRButton low"));
    digitalWrite(SIM808_PWRKEY, LOW);
    delay(2000);
    Serial.println(F("PWRButton high"));
    digitalWrite(SIM808_PWRKEY, HIGH);
}

void sim808_reset(){
    Serial.println(F("Sim808 reset start"));
    digitalWrite(SIM808_RESET, HIGH);
    delay(300);
    Serial.println(F("Sim808 reset done"));
    digitalWrite(SIM808_RESET, LOW);
    sim808_power_on();
}

void sim808_begin(unsigned long baud){
    // pin setup
    pinMode(SIM808_PWRKEY, OUTPUT);
    pinMode(SIM808_STATUS, INPUT);
    pinMode(SIM808_RESET, OUTPUT);
    sim808_power_on();
    fonaSerial->begin(baud);
    if (! sim808.begin(*fonaSerial)) {
        Serial.println(F("GSM module serial connection failed"));
        delay(5*1000);
    }
    sim808_print_imei();
    sim808_wait_for_connection();
    sim808_send_custom_config();
    sim808_delete_all_sms();
}

void sim808_read_rssi(){
    uint8_t n = sim808.getRSSI();
    int8_t r;

    Serial.print(F("RSSI = ")); Serial.print(n); Serial.print(": ");
    if (n == 0) r = -115;
    if (n == 1) r = -111;
    if (n == 31) r = -52;
    if ((n >= 2) && (n <= 30)) {
      r = map(n, 2, 30, -110, -54);
    }
    Serial.print(r); Serial.println(F(" dBm"));
}

// blocking function
void sim808_wait_for_connection(){
    uint8_t status;
    int count = 0;
    do{
        count++;
        delay(2000);
        status = sim808.getNetworkStatus();
        Serial.print(F("Network status: "));
        switch(status) {
            case NETWORK_STATUS_NOT_REGISTERED:
                Serial.println(F("Not registered"));
                break;
            case NETWORK_STATUS_REGISTERED:
                Serial.println(F("Registered (home)"));
                break;
            case NETWORK_STATUS_SEARCHING:
                Serial.println(F("Not registered (searching)"));
                break;
            case NETWORK_STATUS_DENIED:
                Serial.println(F("Denied"));
                break;
            case NETWORK_STATUS_UNKNOWN:
                Serial.println(F("Unknown"));
                break;
            case NETWORK_STATUS_ROAMING:
                Serial.println(F("Registered roaming"));
                break;
        }
    }while(status != NETWORK_STATUS_REGISTERED);
}

void sim808_send_custom_config(){
    uint16_t timeout = 1000;
    //set up the FONA to send a +CMTI notification when an SMS is received
    if (!sim808.sendCheckReply(F("AT+CNMI=2,1"), F("OK"), timeout))
        Serial.print("AT+CNMI=2,1 failed");
    // set sms mode to GSM
    if (!sim808.sendCheckReply(F("AT+CSCS=\"GSM\""), F("OK"), timeout))
        Serial.print("AT+CSCS=\"GSM\" failed");
}


void sim808_delete_sms(int slot){
    if (sim808.deleteSMS(slot)) {
        Serial.println(F("OK!"));
    } else {
        Serial.print(F("Couldn't delete SMS in slot ")); Serial.println(slot);
        // sim808->print(F("AT+CMGD=?\r\n"));
    }
}

void sim808_delete_all_sms(){
    Serial.println(F("deleting all SMS"));
    for(int i = 1; i <= SIM808_SMS_SLOT_COUNT; i++){
        sim808_delete_sms( i);
    }
}

void sim808_print_imei(){
    char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
    uint8_t imeiLen = sim808.getIMEI(imei);
    if (imeiLen > 0) {
        Serial.print("GSM module IMEI: ");
        Serial.println(imei);
    }
}

void sim808_send_command(char *send) {
    Serial.print("send command: ");
    Serial.println(send);
    fonaSerial->print(send);
    fonaSerial->readStringUntil('\r');
    Serial.println("------");
}

void sim808_send_test_sms(){
    char number[] = "123456789";
    char sms_text[] = "test 1234";
    Serial.println("send_test_sms wait 3s");
    delay(5000);
    Serial.println("send_test_sms try sending");
    int tryouts = 5;
    while(tryouts--) {
        if (!sim808.sendSMS(&number[0], &sms_text[0])) {
            Serial.println(F("Failed"));
            delay(50);
        } else {
            Serial.println(F("Sent!"));
            break;
        }
    }
}

void sim808_loop(){
    char* bufPtr = fonaNotificationBuffer;
    if (sim808.available()){
        int slot = 0;            //this will be the slot number of the SMS
        int charCount = 0;
        //Read the notification into fonaInBuffer
        do {
            *bufPtr = sim808.read();
            Serial.write(*bufPtr);
            delay(1);
        } while ((*bufPtr++ != '\n') && (sim808.available()) && (++charCount < (sizeof(fonaNotificationBuffer) - 1)));

        //Add a terminal NULL to the notification string
        *bufPtr = 0;

        //Scan the notification string for an SMS received notification.
        //  If it's an SMS message, we'll get the slot number in 'slot'
        if (1 == sscanf(fonaNotificationBuffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) {
            Serial.print("SMS SLOT: ");
            Serial.println(slot);

            char smsSender[SMS_PHONE_NUMBER_LENGHT];  //we'll store the SMS sender number in here
            // Retrieve SMS sender address/phone number.
            if (!sim808.getSMSSender(slot, smsSender, SMS_PHONE_NUMBER_LENGHT-1)) {
                Serial.println("SMS READ SENDER FAILED - NO MSG IN SLOT");
                Serial.print("SMS FROM: ");
                Serial.println(smsSender);
            }

            // Retrieve SMS value.
            uint16_t smslen;
            if (sim808.readSMS(slot, smsBuffer, 250, &smslen)) { // pass in buffer and max len!
                SMS new_sms;
                strcpy(new_sms.sender, smsSender);
                strcpy(new_sms.msg, smsBuffer);
                SMS_Queue.push(&new_sms);
                Serial.print("SMS MSG : ");
                Serial.println(smsBuffer);
            }else{
                Serial.println("SMS READ MSG FAILED");
            }

            // delete the original msg after it is processed
            //   otherwise, we will fill up all the slots
            //   and then we won't be able to receive SMS anymore
            sim808_delete_sms( slot);
        }
    }
}