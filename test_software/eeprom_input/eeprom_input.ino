#include "octopus_setting.h"
#include "message_data.h"
#include "db_man.h"
#include "byte_utility.h"

#include <EEPROM.h>
#include <PN532.h>
#include <PN532_I2C.h>
#include <PN532_debug.h>


enum State{PRE_CMD, WAIT_CMD, 
READ_NFC_CARD,
PRE_INPUT_CARD, MAN_INPUT_CARD, POST_INPUT_CARD, 
PRE_SHOW_INDIV_DATA, POST_SHOW_INDIV_DATA, 
DUMP_ALL_DATA, WRITE_DEMO_ENTRY};

enum In_card_q{ICQ_NULL, PRE_ICQ_Q1, POST_ICQ_Q1, PRE_ICQ_Q2, POST_ICQ_Q2, PRE_ICQ_Q3, POST_ICQ_Q3, ICQ_DONE}; //Input Card Question

//Global items
State sys_state = PRE_CMD;

In_card_q icq_state = ICQ_NULL;

//Input card question (ICQ) store variables
String icq_input_name;
int icq_input_slot;
uint8_t icq_card_idm[8];

int scq_input_slot;

DB_man db;

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
unsigned long card_read_prev_time;


void show_help()
{
    Serial.print("\n");
    Serial.println("*******  NFC Locker Identity Input   *******");
    Serial.println("*                                          *");
    Serial.println("*                                          *");
    Serial.println("*  1. Input Identity                       *");
    Serial.println("*  2. View Identify                        *");
    Serial.println("*  3. Dump All Record                      *");
    Serial.println("*  4. Write Demo                           *");
    Serial.println("*  8. Print Platform Data                  *");
    Serial.println("*  9. Show Help                            *");
    Serial.println("*                                          *");
    Serial.println("********************************************");
}
void show_platform_data()
{
    Serial.print("\n");

    Serial.print("EEPROM Size: ");
    Serial.print(EEPROM.length());Serial.println(" bytes");

    Serial.print("(card_id_t) size: ");
    Serial.print(sizeof(card_id_t));Serial.println(" bytes");

    Serial.print("(prog_conf) size: ");
    Serial.print(sizeof(prog_conf));Serial.println(" bytes");

    Serial.print("(card_obj) size: ");
    Serial.print(sizeof(card_obj));Serial.println(" bytes");
}

void setup()
{
    Serial.begin(115200);
    Serial.print("\n");

    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata) // If no nfc module detected
    {
        Serial.print(WORD_NFC_INI_ERROR);
        while(1); //halt
    }
    else
    {
        //Setup NFC config
        nfc.setPassiveActivationRetries(0xFF);
        nfc.SAMConfig();

        //Print NFC Module data For Debug
        //Prepare data string
        String nfc_type = String(WORD_NFC_FOUND) + String(((versiondata >> 24) & 0xFF), HEX);
        String nfc_firmware = String(WORD_NFC_SHOW_FIRMWARE) + String(((versiondata >> 16) & 0xFF), DEC) + String(".") + String(((versiondata >> 8) & 0xFF), DEC);

        // Got ok data, print it out!
        Serial.println(nfc_type);
        Serial.println(nfc_firmware);
    }


    show_help();
}

void loop()
{
    switch(sys_state)
    {
        case PRE_CMD:
            Serial.print(WORD_PRE_CMD);
            sys_state = WAIT_CMD;
        break;

        case WAIT_CMD:
        {
            char input_cmd = 0;
            if(Serial.available() > 0)
            {
                input_cmd = Serial.read();

                switch(input_cmd)
                {
                    case '1':
                        icq_state = PRE_ICQ_Q1;
                        sys_state = PRE_INPUT_CARD;
                    break;

                    case '2':
                        sys_state = PRE_SHOW_INDIV_DATA;
                    break;

                    case '3':
                        sys_state = DUMP_ALL_DATA;
                    break;

                    case '4':
                        sys_state = WRITE_DEMO_ENTRY;
                    break;

                    case '8':
                        show_platform_data();
                        sys_state = PRE_CMD; 
                    break;

                    case '9':
                        show_help();
                        sys_state = PRE_CMD; 
                    break;

                    default: //Unknow command
                        Serial.print(WORD_UNKNOWN_CMD);//Print
                        sys_state = PRE_CMD;              
                    break;
                }
            }

        }
        break;

        case PRE_INPUT_CARD:

            switch (icq_state)
            {
                case PRE_ICQ_Q1:
                    Serial.println("Welcome to input card mode");
                    Serial.println("What is the name of card? (Max Length=10)");
                    Serial.print("Name> ");

                    icq_state = POST_ICQ_Q1;


                break;
                    
                case POST_ICQ_Q1:
                    if(Serial.available() > 0)
                    {   
                        icq_input_name = Serial.readString();
                        icq_input_name = icq_input_name.substring(0,9);
                        Serial.println("\nYour Input Name is \""+icq_input_name+"\"");

                        icq_state = PRE_ICQ_Q2;
                    }
                    break;
                break;

                case PRE_ICQ_Q2:
                    Serial.println("What slot you want to Save? (0-9)");
                    Serial.print("Slot> ");

                    icq_state = POST_ICQ_Q2;
                break;
                    
                case POST_ICQ_Q2:
                    if(Serial.available() > 0)
                    {   

                        icq_input_slot = Serial.readString().toInt();
                        Serial.print("\nYour selected slot is \"");
                        Serial.print(icq_input_slot);
                        Serial.println("\"");

                        icq_state = PRE_ICQ_Q3;
                    }
                    break;
                break;
                
                case PRE_ICQ_Q3:
                    Serial.println("\nHow to Input Card ID?");
                    Serial.println("(1) Card Reader? / (2) Manuel Input?");
                    Serial.print("CMD> ");

                    icq_state = POST_ICQ_Q3;                  
                    break;

                

                case POST_ICQ_Q3:
                    if(Serial.available() > 0)
                    {   
                        String temp;
                        temp = Serial.readString();
                        Serial.println("\nYour  selection is \""+temp+"\"");

                        switch(temp.charAt(0))
                        {
                            case '1':
                                sys_state = READ_NFC_CARD;
                                Serial.println("Please Tap your card to the card reader");
                            break;

                            case '2':
                                sys_state = MAN_INPUT_CARD;
                            break;
                        }
                    }
                    
                break;

                default:
                    break;
            }
            
            
            
        break;

        case READ_NFC_CARD:
        {
            if((millis() - card_read_prev_time) > OCTOPUS_PROCESSING_INTERVAL)
            {
                uint8_t ret;
                uint16_t systemCode = OCTOPUS_SYSTEM_CODE;
                uint8_t requestCode = OCTOPUS_REQUEST_CODE;       // System Code request
                uint8_t idm[8];
                uint8_t pmm[8];
                uint16_t systemCodeResponse;
                //Read NFC tag
                ret = nfc.felica_Polling(systemCode, requestCode, idm, pmm, &systemCodeResponse, 5000); 

                //Found an Octopus Card
                if (ret == 1)
                {
                    //Send a request to end the apple pay card reading
                    uint16_t serviceCodeList[1];
                    uint16_t returnKey[1];
                    uint16_t blockList[1];
                    serviceCodeList[0]=OCTOPUS_BALANCE_SERVICE_CODE;
                    blockList[0] = OCTOPUS_BALANCE_SERVICE_BLOCK;
                    ret = nfc.felica_RequestService(1, serviceCodeList, returnKey); //Request the Service
                    nfc.felica_Release();

                    Serial.println("\nYour Card ID is \""+convBytesToString(idm, 8)+"\"");

                    memcpy(icq_card_idm, idm, 8*sizeof(uint8_t));

                    sys_state = POST_INPUT_CARD;
                }
                card_read_prev_time = millis();
            }
            
        }
        break;

        case MAN_INPUT_CARD:
            Serial.println("Function NOT yet Implemented");
            sys_state = PRE_CMD;
            // sys_state = POST_INPUT_CARD;
        break;

        case POST_INPUT_CARD:
            Serial.println("You Input data is:");
            Serial.println("Name: "+ icq_input_name);
            Serial.print("Slot: "); Serial.println(icq_input_slot);
            Serial.println("Card ID: "+convBytesToString(icq_card_idm,8));

            card_obj temp_obj;
            memcpy(temp_obj.card_id.b8a, icq_card_idm, 8);
            memcpy(temp_obj.name, icq_input_name.c_str(), icq_input_name.length());

            db.write_entry(icq_input_slot, temp_obj);

            //Clean variable
            icq_state = ICQ_NULL;
            icq_input_name = "";
            icq_input_slot = -1;
            memset(icq_card_idm, 0, 8);
            
            sys_state = PRE_CMD;
        break;

        case PRE_SHOW_INDIV_DATA:
        {
            Serial.println("Which slot you want to show? (0-9)");
            Serial.print("Slot> ");

            sys_state = POST_SHOW_INDIV_DATA;
        }
        break;
        case POST_SHOW_INDIV_DATA:  
        {
            if(Serial.available() > 0)
            {   

                scq_input_slot = Serial.readString().toInt();
                Serial.print("\nYour selected slot is \"");
                Serial.print(scq_input_slot);
                Serial.println("\"");

                card_obj obj;
                db.getCard(scq_input_slot, obj);

                Serial.println("query result:");
                Serial.print("Name:");Serial.println(obj.name);
                Serial.print("Card ID:");Serial.println(convBytesToString(obj.card_id.b8a, 8));
                sys_state = PRE_CMD;
            }
        }
        break;

        case WRITE_DEMO_ENTRY:
        {
            card_obj temp = {{0x12,0x34,0x45,0x78,0x92}, "Hello2"};
            
            Serial.println("Input Demo record");
            Serial.print("Name:");Serial.println(temp.name);
            Serial.print("Card ID:");Serial.println(convBytesToString(temp.card_id.b8a, 8));

            //TODO: uncomment below before deploy
            //db.write_entry(0, temp); 
            sys_state = PRE_CMD;


            Serial.println("Done!");
        }
        break;

        case DUMP_ALL_DATA:
        {
            Serial.println("\nNot yet Implemented");
            sys_state = PRE_CMD;
        }
        break;
    }
}