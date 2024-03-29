//
// Created by debauer on 16.11.21.
//

#ifndef SIM808_DEFINES_H
#define SIM808_DEFINES_H

#define BAUD_SERIAL 115200
#define BAUD_SIM808 4800

#define SIM808_RESET_RELAY 23
#define SIM808_RESET 18
#define SIM808_PWRKEY 21	///< SIM808 PWRKEY
#define SIM808_STATUS 34	///< SIM808 STATUS

#define TIMER_1S 1000000
#define TIMER_1MS 1000

#define HEATER_RELAY_PIN 1

#ifdef LOG_DEBUG
#define LOG_DEBUG(s) Serial.println("\u001b[0m DEBUG \u001b[0m" s)
#else
#define LOG_DEBUG(s)
#endif

#ifdef LOG_WARN
#define LOG_WARN(s) Serial.println("\u001b[35m WARN \u001b[0m" s)
#else
#define LOG_WARN(s)
#endif

#ifdef LOG_ERROR
#define LOG_ERROR(s) Serial.println("\u001b[31m ERROR \u001b[0m" s)
#else
#define LOG_ERROR(s)
#endif

#endif //SIM808_DEFINES_H
