//
// Created by debauer on 16.11.21.
//

#ifndef SIM808_H
#define SIM808_H

class SIM808;

#include "Arduino.h"
#include "Adafruit_FONA.h"


#define SMS_QUEUE_SIZE 20
#define SMS_LENGHT 250
#define SMS_PHONE_NUMBER_LENGHT 32
#define SIM808_SMS_SLOT_COUNT 8
#define SIM808_SMS_BUFFERSIZE 1024

#ifdef UNIT_TEST
    #define FONA_LIB fona_dummy
#else
    #define FONA_LIB Adafruit_FONA
#endif

enum SMS_Status{
    SMS_UNDEF,
    SMS_REC_UNREAD, //  Received unread messages
    SMS_REC_READ, //  Received read messages
    SMS_STO_UNSENT, //  Stored unsent messages
    SMS_STO_SENT, //  Stored sent messages
    SMS_ALL, //  All messages
};

struct SMS {
  int slot;
  SMS_Status status;
  char number[20];
  char message[180];
  char datetime[20];
} ;

enum Network_State {
    NOT_REGISTERED, REGISTERED, SEARCHING, DENIED, UNKNOWN, ROAMING
};



class SIM808 {
public:
    // FUNCTIONS
    void reset(bool soft = false);
    void power_on();
    void begin(FONA_LIB *adafruit_fona, HardwareSerial *serial, unsigned long baud, int pin_power, int pin_status, int pin_reset, int pin_reset_relay);
    void loop();

    int get_rssi();
    Network_State get_network_state();
    void print_imei();

    // SMS
    void poll_sms();
    void delete_sms(int slot);
    void send_sms(char *number, char *sms_text);

protected:

    // FUNCTIONS
    void begin_fona();

    int update_rssi();
    Network_State update_network_status();
    void update_imei();

    // SMS
    void parse_sms(uint16_t msg_size);

    // VARS
    Network_State network_state;
    int rssi;
    char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!

    // PINS
    int _pin_power;
    int _pin_status;
    int _pin_reset_relay;
    int _pin_reset;

    HardwareSerial *fonaSerial;
    Adafruit_FONA *fona;
};

#endif //SIM808_H
