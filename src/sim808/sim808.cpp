//
// Created by debauer on 16.11.21.
//

#include "sim808.h"

void SIM808::power_on() const{
    Serial.println(F("SIM808 power_on start"));
    digitalWrite(_pin_power, LOW);
    delay(2000);
    digitalWrite(_pin_power, HIGH);
    Serial.println(F("SIM808 power_on end"));
}

void SIM808::reset(bool soft){
    Serial.println(F("Sim808 reset start"));
    begin_fona();
    Serial.println(F("Sim808 reset done"));
}

void SIM808::begin_fona(){
    digitalWrite(_pin_power, LOW);
    delay(500);
    digitalWrite(_pin_power, HIGH);
    power_on();
    if (! fona->begin(*fonaSerial)) {
        Serial.println(F("GSM module serial connection failed"));
        delay(5*1000);
    }
    update_imei();
    print_imei();
}

void SIM808::begin(FONA_LIB *adafruit_fona, HardwareSerial *serial, unsigned long baud, int pin_power, int pin_status, int pin_reset, int pin_reset_relay){
    // pin setup
    _pin_power = pin_power;
    _pin_status = pin_status;
    _pin_reset_relay = pin_reset_relay;
    _pin_reset = pin_reset;
    fona = adafruit_fona;
    fonaSerial = serial;
    pinMode(_pin_power, OUTPUT);
    pinMode(_pin_status, INPUT);
    pinMode(_pin_reset, OUTPUT);
    pinMode(_pin_reset_relay, OUTPUT);
    fonaSerial->begin(baud);
    begin_fona();
    //wait_for_connection();
    //send_custom_config();
    //delete_all_sms();
}

int SIM808::update_rssi(){
    uint8_t n = fona->getRSSI();
    int r;
    if (n == 0) r = -115;
    if (n == 1) r = -111;
    if (n == 31) r = -52;
    if ((n >= 2) && (n <= 30)) {
      r = (int) map(n, 2, 30, -110, -54);
    }
    _rssi = r;
    return _rssi;
}

int SIM808::get_rssi() const{
    return _rssi;
}

Network_State SIM808::update_network_status(){
    _network_state = static_cast<Network_State>(fona->getNetworkStatus());
    return _network_state;
}

Network_State SIM808::get_network_state(){
    return _network_state;
}

void SIM808::delete_sms(int slot){
    if (fona->deleteSMS(slot)) {
        Serial.println(F("delete_sms OK!"));
    } else {
        Serial.print(F("delete_sms Couldn't delete SMS in slot ")); Serial.println(slot);
        // sim808->print(F("AT+CMGD=?\r\n"));
    }
}

void SIM808::update_imei(){
    fona->getIMEI(_imei);
}

void SIM808::print_imei(){
    Serial.print("SIM808 IMEI: ");
    Serial.println(_imei);
}

void SIM808::send_sms(char *number, char *sms_text){
    Serial.println("send_test_sms wait 3s");
    delay(5000);
    Serial.println("send_test_sms try sending");
    int tryouts = 5;
    while(tryouts--) {
        if (!fona->sendSMS(&number[0], &sms_text[0])) {
            Serial.println(F("Failed"));
            delay(50);
        } else {
            Serial.println(F("Sent!"));
            break;
        }
    }
}


// incoming message looks like:
// +CMGL: 11,"REC UNREAD","+49123456789","","21/11/15,21:47:27+04"\n
// Test 133\n
// \n

void SIM808::clear_sms_cache(){
    // we check for
    for(int i = 0; i < SMS_QUEUE_SIZE; i++){
        _sms[i].status = SMS_UNDEF;
        _sms[i].slot = 0;
    }
}

void SIM808::parse_sms(uint16_t msg_size){
    int sms_count = 0;
    clear_sms_cache();
    for(int i = 0; i < msg_size; i++){
        if (strncmp(&_sms_buffer[i], "+CMGL", 5) == 0) {
            // found sms
            Serial.print(F("PARSE_SMS: found at "));
            Serial.println(i);

            // ======= SLOT =======
            i = i + 7; // set on first number of slot
            _sms[sms_count].slot = 0;
            while(_sms_buffer[i] >= '0' && _sms_buffer[i] <= '9' ){
                _sms[sms_count].slot = _sms[sms_count].slot * 10 + _sms_buffer[i] - '0';
                i++;
            }
            Serial.print(F("PARSE_SMS: slot "));
            Serial.println(_sms[sms_count].slot);


            // ======= SMS STATUS =======
            i = i + 2;
            Serial.print(F("PARSE_SMS: status "));

            if (_sms_buffer[i] == 'R'){
                if(_sms_buffer[i+4] == 'U'){
                    _sms[sms_count].status = SMS_REC_UNREAD;
                    Serial.println("SMS_REC_UNREAD");
                    i = i + 10;
                }
                if(_sms_buffer[i+4] == 'R'){
                    _sms[sms_count].status = SMS_REC_READ;
                    Serial.println("SMS_REC_READ");
                    i = i + 8;
                }
            }
            else if (_sms_buffer[i] == 'S'){
                if(_sms_buffer[i+4] == 'U'){
                    _sms[sms_count].status = SMS_STO_UNSENT;
                    Serial.println("SMS_STO_UNSENT");
                    i = i + 10;
                }
                if(_sms_buffer[i+4] == 'S'){
                    _sms[sms_count].status = SMS_STO_SENT;
                    Serial.println("SMS_STO_SENT");
                    i = i + 8;
                }
            }
            else if (_sms_buffer[i] == 'A'){
                _sms[sms_count].status = SMS_ALL;
                Serial.println("SMS_ALL");
                i = i + 7;
            }else{
                _sms[sms_count].status = SMS_UNDEF;
                Serial.println("UNKNOWN");
            }


            // ======= TELEPHONE NUMBER =======
            i = i + 3;
            for(int j = 0; _sms_buffer[i] != '"'; j++){
                _sms[sms_count].number[j] = _sms_buffer[i++];
                _sms[sms_count].number[j+1] = 0;
            }
            Serial.print(F("PARSE_SMS: number "));
            Serial.println(_sms[sms_count].number);
            i = i+3;
            if(_sms_buffer[i] == '"') i = i+3;
            else{
                while(_sms_buffer[i++] != '"');
                i = i+2;
            }

            // ======= DATE / TIME =======
            for(int j = 0; _sms_buffer[i] != '"'; j++){
                _sms[sms_count].datetime[j] = _sms_buffer[i];
                _sms[sms_count].datetime[j+1] = 0;
                i++;
            }
            Serial.print(F("PARSE_SMS: date/time "));
            Serial.println(_sms[sms_count].datetime);
            i = i+2;

            // ======= MESSAGE =======
            for(int j = 0; _sms_buffer[i] != '\n'; j++){
                _sms[sms_count].message[j] = _sms_buffer[i++];
                _sms[sms_count].message[j+1] = 0;
            }

            Serial.print(F("PARSE_SMS: message "));
            Serial.println(_sms[sms_count].message);
            sms_count++;
        }
    }
}

void SIM808::analyze_sms(){
    for(int i = 0; i < SMS_QUEUE_SIZE; i++){
        if(_sms[i].slot){
            // delete_sms(_sms[i].slot);
        }
    }
}

void SIM808::poll_sms(){
    uint16_t replay_lenght = 0;
    int timeout = TIMEOUT;
    for(int i = 0; i<SIM808_SMS_BUFFERSIZE; i++ ){
         _sms_buffer[i] = 0;
    }
    while(fonaSerial->available() > 0) {
        fonaSerial->read();
    }
    if (fona->sendCheckReply(F("AT+CMGF=1"), F("OK")), 2000)
        Serial.println(F("FONA Text Mode enabled"));

    fonaSerial->print("AT+CMGL=\"ALL\"\n");
    delay(10);
    while (timeout--) {
        if (replay_lenght >= SIM808_SMS_BUFFERSIZE - 1) {
            Serial.println("======= REPLYBUFFER FULL =======");
            break;
        }
        while (fonaSerial->available()) {
            char c = (char) fonaSerial->read();
            if (c == '\r')
                continue;
            _sms_buffer[replay_lenght] = c;
            replay_lenght++;
            timeout = TIMEOUT;
        }
        delay(1);
    }
    parse_sms(replay_lenght);
    analyze_sms();
}

void SIM808::loop(){
    update_network_status();
    update_rssi();
    if(_network_state == REGISTERED){
        // read sms
    }
}