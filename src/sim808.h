//
// Created by debauer on 16.11.21.
//

#ifndef SIM808_H
#define SIM808_H

#include "global.h"

#define NETWORK_STATUS_NOT_REGISTERED 0
#define NETWORK_STATUS_REGISTERED 1
#define NETWORK_STATUS_SEARCHING 2
#define NETWORK_STATUS_DENIED 3
#define NETWORK_STATUS_UNKNOWN 4
#define NETWORK_STATUS_ROAMING 5

#define SIM808_SMS_SLOT_COUNT 8

void sim808_wait_for_connection();
void sim808_reset();
void sim808_power_on();
void sim808_begin(unsigned long baud);
void sim808_delete_sms(int slot);
void sim808_delete_all_sms();
void sim808_loop();
void sim808_send_custom_config();
void sim808_print_imei();
void sim808_send_test_sms();
void sim808_send_command(char *send);

#endif //SIM808_H
