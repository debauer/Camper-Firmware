//
// Created by debauer on 23.12.21.
//

//g++  7.4.0

#include <iostream>
#include <cstring>

#define BUFFERSIZE 350
#define TIMEOUT 1000

enum SMS_Status{
    SMS_REC_UNREAD, //  Received unread messages
    SMS_REC_READ, //  Received read messages
    SMS_STO_UNSENT, //  Stored unsent messages
    SMS_STO_SENT, //  Stored sent messages
    SMS_ALL //  All messages
};

struct SMS {
  int slot;
  SMS_Status status;
  char number[20];
  char message[180];

} ;

char nr[20];
char datetime[20];
char msg[180];
int slot = 0;

SMS incomming_sms[10];

const char *SMSreplybuffer =
    "+CMGL: 11,\"REC UNREAD\",\"++49123456789\",\"\",\"21/11/15,21:47:27+04\"\n"
    "Test 133\n"
    "\n"
    "+CMGL: 232,\"STO SENT\",\"++49123456789\",\"\",\"21/11/15,21:54:11+04\"\n"
    "Hahaha\n"
    "\n"
    "+CMGL: 3212,\"ALL\",\"++49123456789\",\"\",\"21/11/15,22:58:20+04\"\n"
    "Adfa\n"
    "\n"
    "+CMGL: 4,\"REC UNREAD\",\"++49123456789\",\"\",\"41/11/16,17:12:13+04\"\n"
    "Test\n"
    "\n"
    "+CMGL: 5,\"REC READ\",\"++49123456789\",\"\",\"31/11/16,17:19:35+04\"\n"
    "Asdf\n"
    "\n"
    "+CMGL: 6,\"STO UNSENT\",\"++49123456789\",\"asdadssad\",\"11/11/16,17:20:55+04\"\n"
    "Asdf\n";

#define RED_BOLD(x) "\u001b[31m"#x
#define FORMAT_RESET(x) "\u001b[0m"#x

int main(){
    int parseState = 0;
    int startid = 0;
    int endid = 0;
    for(int i = 0; i< 0; i++){
        if (strncmp(&SMSreplybuffer[i], "+CMGL", 5) == 0) {
            // found sms
            std::cout << "YELLOW_BOLD at: " << i << "\n";

            // ======= SLOT =======
            i = i + 7; // set on first number of slot
            while(SMSreplybuffer[i] >= '0' && SMSreplybuffer[i] <= '9' ){
                slot = slot * 10 + SMSreplybuffer[i] - '0';
                i++;
            }
            std::cout << "slot: " << slot << "\n";


            // ======= SMS STATUS =======
            i = i + 2;
            if (SMSreplybuffer[i] == 'R'){
                if(SMSreplybuffer[i+4] == 'U'){
                    std::cout << "status: " << "SMS_REC_UNREAD" << "\n";
                    i = i + 10;
                }
                if(SMSreplybuffer[i+4] == 'R'){
                    std::cout << "status: " << "SMS_REC_READ" << "\n";
                    i = i + 8;
                }
            }
            if (SMSreplybuffer[i] == 'S'){
                if(SMSreplybuffer[i+4] == 'U'){
                    std::cout << "status: " << "SMS_STO_UNSENT" << "\n";
                    i = i + 10;
                }
                if(SMSreplybuffer[i+4] == 'S'){
                    std::cout << "status: " << "SMS_STO_SENT" << "\n";
                    i = i + 8;
                }
            }
            if (SMSreplybuffer[i] == 'A'){
                std::cout << "status: " << "SMS_ALL" << "\n";
                i = i + 7;
            }

            // ======= TELEPHONE NUMBER =======
            i = i + 3;
            for(int j = 0; SMSreplybuffer[i] != '"'; j++){
                nr[j] = SMSreplybuffer[i++];
            }
            std::cout << "Number: " << nr << "\n";
            i = i+3;
            if(SMSreplybuffer[i] == '"') i = i+3;
            else{
                while(SMSreplybuffer[i++] != '"');
                i = i+2;
            }

            // ======= DATE / TIME =======
            for(int j = 0; SMSreplybuffer[i] != '"'; j++){
                datetime[j] = SMSreplybuffer[i];
                i++;
            }
            std::cout << "datetime: " << datetime << "\n";
            i = i+2;

            // ======= MESSAGE =======
            for(int j = 0; SMSreplybuffer[i] != '\n'; j++){
                msg[j] = SMSreplybuffer[i++];
            }

            std::cout << "msg: " << msg << "\n";

            std::cout << "\n";
        }

    }
}