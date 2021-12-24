//
// Created by debauer on 23.12.21.
//

#ifndef SIM808_STRUCT_H
#define SIM808_STRUCT_H

struct SMS {
  int slot;
  SMS_Status status;
  char number[20];
  char message[180];
  char datetime[20];
} ;

#endif //SIM808_STRUCT_H
