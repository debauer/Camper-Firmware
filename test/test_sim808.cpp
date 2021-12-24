#include <Arduino.h>
#include <unity.h>
#include "sim808.h"

void test_sim808_imei() {
    char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
    char testimei[] = "testimei";
    sim808.getIMEI(imei);
    TEST_ASSERT(strcmp(testimei, imei) == 0);
}

void test_sim808_getNetworkStatus() {
    uint8_t status = sim808.getNetworkStatus();
    TEST_ASSERT_EQUAL(1, status);
}

void test_sim808_getSMSSender() {
    char sender[16] = {0}; // MUST use a 16 character buffer for IMEI!
    char testsender[] = "0123456789";
    TEST_ASSERT_TRUE(sim808.getSMSSender(1, sender, SMS_PHONE_NUMBER_LENGHT - 1));
    TEST_ASSERT(strcmp(sender, testsender) == 0);
}

void test_sim808_getSMS() {
    char sms[16] = {0}; // MUST use a 16 character buffer for IMEI!
    uint16_t readlen = 0;
    char testsms[] = "test sms";
    TEST_ASSERT_TRUE(sim808.readSMS(1, sms, SMS_LENGHT, &readlen));
    TEST_ASSERT(strcmp(sms, testsms) == 0);
    TEST_ASSERT_EQUAL(8, readlen);
}

void test_sim808_getRSSI() {
    TEST_ASSERT_EQUAL(15, sim808.getRSSI());
    TEST_ASSERT_EQUAL(-84, sim808_read_rssi());
}

void test_sim808_read() {
    fonaSerial->write('s');
    delay(100);
    Serial.println(fonaSerial->available());
    int ret = sim808.read();
    TEST_ASSERT_EQUAL('s', ret);

    fonaSerial->write("+CMTI: \"SM\",1\n");
    delay(100);
    for(int i = 0; i< 20; i++){
        sim808_loop();
    }
}

void setup() {
    delay(2000);
    sim808.begin(*fonaSerial);
    delay(100);
    // flush input buffer
    while(fonaSerial->available() > 0) {
        fonaSerial->read();
    }
    UNITY_BEGIN();
}

void loop() {
    RUN_TEST(test_sim808_read);
    RUN_TEST(test_sim808_imei);
    RUN_TEST(test_sim808_getNetworkStatus);
    RUN_TEST(test_sim808_getSMSSender);
    RUN_TEST(test_sim808_getSMS);
    RUN_TEST(test_sim808_getRSSI);
    UNITY_END(); // stop unit testing
}