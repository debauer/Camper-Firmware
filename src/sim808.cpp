//
// Created by debauer on 16.11.21.
//

#include "sim808.h"

void SIM808::power_on(){
    Serial.println(F("SIM808 power_on start"));
    digitalWrite(SIM808_PWRKEY, LOW);
    delay(2000);
    digitalWrite(SIM808_PWRKEY, HIGH);
    Serial.println(F("SIM808 power_on end"));
}

void SIM808::reset(bool soft){
    Serial.println(F("Sim808 reset start"));
    begin_fona();
    Serial.println(F("Sim808 reset done"));
}

void SIM808::begin_fona(){
    digitalWrite(SIM808_PWRKEY, LOW);
    delay(500);
    digitalWrite(SIM808_PWRKEY, HIGH);
    power_on();
    if (! fona.begin(*fonaSerial)) {
        Serial.println(F("GSM module serial connection failed"));
        delay(5*1000);
    }

    print_imei();
}

void SIM808::begin(unsigned long baud){
    // pin setup
    pinMode(SIM808_PWRKEY, OUTPUT);
    pinMode(SIM808_STATUS, INPUT);
    pinMode(SIM808_RESET, OUTPUT);
    pinMode(SIM808_RESET_RELAY, OUTPUT);
    fonaSerial->begin(baud);
    begin_fona();
    //wait_for_connection();
    //send_custom_config();
    //delete_all_sms();
}

void SIM808::update_rssi(){
    uint8_t n = fona.getRSSI();
    int8_t r;
    if (n == 0) r = -115;
    if (n == 1) r = -111;
    if (n == 31) r = -52;
    if ((n >= 2) && (n <= 30)) {
      r = map(n, 2, 30, -110, -54);
    }
    system_status.rssi = r;
}

Network_State update_network_status(){
    Network_State s;
    s = static_cast<Network_State>(fona.getNetworkStatus());
    system_status.setNetworkState(s);
    return s;
}

// blocking function
void SIM808::wait_for_connection(){
    Network_State state;
    int count = 0;
    do{
        count++;
        delay(2000);
        state = static_cast<Network_State>(fona.getNetworkStatus());
        Serial.print(F("Network status: "));
        switch(state) {
            case NOT_REGISTERED:
                Serial.println(F("Not registered"));
                break;
            case REGISTERED:
                Serial.println(F("Registered (home)"));
                break;
            case SEARCHING:
                Serial.println(F("Not registered (searching)"));
                break;
            case DENIED:
                Serial.println(F("Denied"));
                break;
            default:
            case UNKNOWN:
                Serial.println(F("Unknown"));
                break;
            case ROAMING:
                Serial.println(F("Registered roaming"));
                break;
        }
    }while(state != REGISTERED);
}

// void SIM808::send_custom_config(){
//     uint16_t timeout = 1000;
//     //set up the FONA to send a +CMTI notification when an SMS is received
//     if (!fona.sendCheckReply(F("AT+CNMI=2,1"), F("OK"), timeout))
//         Serial.print("send_custom_config AT+CNMI=2,1 failed");
//
// }


void SIM808::delete_sms(int slot){
    if (fona.deleteSMS(slot)) {
        Serial.println(F("delete_sms OK!"));
    } else {
        Serial.print(F("delete_sms Couldn't delete SMS in slot ")); Serial.println(slot);
        // sim808->print(F("AT+CMGD=?\r\n"));
    }
}

void SIM808::delete_all_sms(){
    Serial.println(F("deleting all SMS"));
    #ifndef SIM808_DUMMY
    for(int i = 1; i <= SIM808_SMS_SLOT_COUNT; i++){
        SIM808::delete_sms( i);
    }
    #endif
}

void SIM808::print_imei(){
    char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
    uint8_t imeiLen = fona.getIMEI(imei);
    if (imeiLen > 0) {
        Serial.print("GSM module IMEI: ");
        Serial.println(imei);
    }
}

void SIM808::send_command(char *send) {
    Serial.print("send command: ");
    Serial.println(send);
    fonaSerial->print(send);
    fonaSerial->readStringUntil('\r');
    Serial.println("------");
}

void SIM808::send_test_sms(){
    char number[] = "123456789";
    char sms_text[] = "test 1234";
    Serial.println("send_test_sms wait 3s");
    delay(5000);
    Serial.println("send_test_sms try sending");
    int tryouts = 5;
    while(tryouts--) {
        if (!fona.sendSMS(&number[0], &sms_text[0])) {
            Serial.println(F("Failed"));
            delay(50);
        } else {
            Serial.println(F("Sent!"));
            break;
        }
    }
}

#define TIMEOUT 1000
char SMS_buffer[SIM808_SMS_BUFFERSIZE];
SMS sms[10];

// incoming message looks like:
// +CMGL: 11,"REC UNREAD","+49123456789","","21/11/15,21:47:27+04"\n
// Test 133\n
// \n

void SIM808::parse_sms(uint16_t msg_size){
    int sms_count = 0;
    for(int i = 0; i < msg_size; i++){
        if (strncmp(&SMS_buffer[i], "+CMGL", 5) == 0) {
            // found sms
            Serial.print(F("PARSE_SMS: found at "));
            Serial.println(i);

            // ======= SLOT =======
            i = i + 7; // set on first number of slot
            sms[sms_count].slot = 0;
            while(SMS_buffer[i] >= '0' && SMS_buffer[i] <= '9' ){
                sms[sms_count].slot = sms[sms_count].slot * 10 + SMS_buffer[i] - '0';
                i++;
            }
            Serial.print(F("PARSE_SMS: slot "));
            Serial.println(sms[sms_count].slot);


            // ======= SMS STATUS =======
            i = i + 2;
            Serial.print(F("PARSE_SMS: status "));

            if (SMS_buffer[i] == 'R'){
                if(SMS_buffer[i+4] == 'U'){
                    Serial.println("SMS_REC_UNREAD");
                    i = i + 10;
                }
                if(SMS_buffer[i+4] == 'R'){
                    Serial.println("SMS_REC_READ");
                    i = i + 8;
                }
            }
            else if (SMS_buffer[i] == 'S'){
                if(SMS_buffer[i+4] == 'U'){
                    Serial.println("SMS_STO_UNSENT");
                    i = i + 10;
                }
                if(SMS_buffer[i+4] == 'S'){
                    Serial.println("SMS_STO_SENT");
                    i = i + 8;
                }
            }
            else if (SMS_buffer[i] == 'A'){
                Serial.println("SMS_ALL");
                i = i + 7;
            }else{
                Serial.println("UNKNOWN");
            }


            // ======= TELEPHONE NUMBER =======
            i = i + 3;
            for(int j = 0; SMS_buffer[i] != '"'; j++){
                sms[sms_count].number[j] = SMS_buffer[i++];
                sms[sms_count].number[j+1] = 0;
            }
            Serial.print(F("PARSE_SMS: number "));
            Serial.println(sms[sms_count].number);
            i = i+3;
            if(SMS_buffer[i] == '"') i = i+3;
            else{
                while(SMS_buffer[i++] != '"');
                i = i+2;
            }

            // ======= DATE / TIME =======
            for(int j = 0; SMS_buffer[i] != '"'; j++){
                sms[sms_count].datetime[j] = SMS_buffer[i];
                sms[sms_count].datetime[j+1] = 0;
                i++;
            }
            Serial.print(F("PARSE_SMS: date/time "));
            Serial.println(sms[sms_count].datetime);
            i = i+2;

            // ======= MESSAGE =======
            for(int j = 0; SMS_buffer[i] != '\n'; j++){
                sms[sms_count].message[j] = SMS_buffer[i++];
                sms[sms_count].message[j+1] = 0;
            }

            Serial.print(F("PARSE_SMS: message "));
            Serial.println(sms[sms_count].message);
            sms_count++;
        }

    }
}

void SIM808::poll_sms(){
    uint16_t replyidx = 0;
    int timeout = TIMEOUT;
    for(int i = 0; i<SIM808_SMS_BUFFERSIZE; i++ ){
         SMS_buffer[i] = 0;
    }
    while(fonaSerial->available() > 0) {
        fonaSerial->read();
    }
    if (fona.sendCheckReply(F("AT+CMGF=1"), F("OK")), 2000)
        Serial.println(F("FONA Text Mode enabled"));

    fonaSerial->print("AT+CMGL=\"ALL\"\n");
    delay(10);
    while (timeout--) {
        if (replyidx >= SIM808_SMS_BUFFERSIZE - 1) {
            Serial.println("======= REPLYBUFFER FULL =======");
            break;
        }
        while (fonaSerial->available()) {
            char c = fonaSerial->read();
            if (c == '\r')
                continue;
            SMS_buffer[replyidx] = c;
            replyidx++;
            timeout = TIMEOUT;
        }
        delay(1);
    }
    //Serial.println("======= REPLYBUFFER =======");
    //for(int i = 0; i<replyidx; i++ ){
    //    Serial.print(SMS_buffer[i]);
    //}
    //Serial.println("");
    //Serial.println("======= REPLYBUFFER END =======");
    parse_sms(replyidx);
}


void SIM808::read_sms(){
    int slot = 0;
    Serial.println("READ_SMS called");
    if (1 == sscanf(recive_buffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) {
        Serial.print("READ_SMS SLOT: ");
        Serial.println(slot);

        // set sms mode to GSM
        if (!fona.sendCheckReply(F("AT+CSCS=\"GSM\""), F("OK"), 2000))
            Serial.println("READ_SMS send_custom_config AT+CSCS=\"GSM\" failed");

        char smsSender[SMS_PHONE_NUMBER_LENGHT];  //we'll store the SMS sender number in here
        // Retrieve SMS sender address/phone number.
        if (!fona.getSMSSender(slot, smsSender, SMS_PHONE_NUMBER_LENGHT - 1)) {
            Serial.println("READ_SMS SMS READ SENDER FAILED - NO MSG IN SLOT");
            Serial.print("READ_SMS SMS FROM: ");
            Serial.println(smsSender);
        }

        // Retrieve SMS value.
        uint16_t smslen;
        if (fona.readSMS(slot, smsBuffer, 250, &smslen)) { // pass in buffer and max len!
            SMS new_sms;
            new_sms.slot = slot;
            new_sms.status = SMS_UNDEF;
            strcpy(new_sms.number, smsSender);
            strcpy(new_sms.message, smsBuffer);
            SMS_Queue.push(&new_sms);
            Serial.print("read_sms SMS MSG : ");
            Serial.println(smsBuffer);
        } else {
            Serial.println("read_sms SMS READ MSG FAILED");
        }

        // delete the original msg after it is processed
        //   otherwise, we will fill up all the slots
        //   and then we won't be able to receive SMS anymore
        delete_sms(slot);
    }
}

char SIM808::read_serial() {
    static char* bufPtr = recive_buffer;
    static int charCount = 0;
    if (fona.available()){ // we have a new charakter in serial buffer
        *bufPtr = fona.read();
        if(*bufPtr == '\n'){
            Serial.println("read_serial found \\n");
            *++bufPtr = 0; //Add a terminal NULL to the notification string
            bufPtr = recive_buffer;
            return 1; // ok
        }
        bufPtr++;
        if (++charCount >= (sizeof(recive_buffer) - 1)){
            Serial.println("read_serial recive_buffer is full");
            return 2; // error
        }
    }
    return 0; // nothing to do
}

void SIM808::find_routine(){
    if(strncmp (recive_buffer,"+CMTI:",6) == 0){
        Serial.print("find_routine: read_sms");
        read_sms();
        return;
    }
    Serial.println("find_routine: ERROR no routine found");
    Serial.print("find_routine: buffer: ");
    Serial.println(recive_buffer);
}

void SIM808::loop(){
    //char return_code = read_serial();
    //if (return_code == 1){
    //    find_routine();
    //}
    update_network_status();
    update_rssi();
    if(system_status.getNetworkState() == REGISTERED){
        // read sms
    }
}