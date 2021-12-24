//
// Created by debauer on 16.11.21.
//

#ifndef SIM808_H
#define SIM808_H

class SIM808;

#include "global.h"

class SIM808 {
public:
    void wait_for_connection();
    void reset(bool soft = false);
    void power_on();
    void begin(unsigned long baud);
    void delete_sms(int slot);
    void delete_all_sms();
    void loop();
    //void send_custom_config();
    void print_imei();
    void send_test_sms();
    void send_command(char *send);
    void update_rssi();
    void poll_sms();
protected:
    void read_sms();
    char read_serial();
    void find_routine();
    void begin_fona();

    char smsBuffer[250];
    char recive_buffer[64];
    void parse_sms(uint16_t msg_size);
};

#endif //SIM808_H
