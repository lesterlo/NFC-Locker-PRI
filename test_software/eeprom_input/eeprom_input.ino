#include "octopus_setting.h"
#include "message_data.h"
#include "db_man.h"
#include "byte_utility.h"

#include <EEPROM.h>
#include <PN532.h>
#include <PN532_I2C.h>
#include <PN532_debug.h>


enum State{PRE_CMD, WAIT_CMD, INPUT_CARD, SHOW_INDIV_DATA, DUMP_ALL_DATA, WRITE_DEMO_ENTRY};

//Global items
State sys_state = PRE_CMD;

DB_man db;

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

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
                        sys_state = INPUT_CARD;
                    break;

                    case '2':
                        sys_state = SHOW_INDIV_DATA;
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

        case INPUT_CARD:
            sys_state = PRE_CMD;
        break;

        case SHOW_INDIV_DATA:
        {
            card_obj obj;
            String print_str;
            db.getCard(0, obj);

            Serial.print("Name:");Serial.println(obj.name);
            Serial.print("Card ID:");Serial.println(convBytesToString(obj.card_id.b8a, 8));
            sys_state = PRE_CMD;
        }
        break;

        case WRITE_DEMO_ENTRY:
        {
            card_obj temp = {{0x12,0x34,0x45,0x78,0x92}, "Hello2"};
            // temp.card_id.b64 = 0x23456789;
            

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