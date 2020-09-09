/* 
* NFC Locker (by Lester Lo)
*
*
*
*/

// Program Parameter
// #define CENSOR_MODE      //Disable showing Card Information
#define SERIAL_OUTPUT       //Show the program status via USB Serial Console

//Include Header
#include "program_setting.h"
#include "octopus_setting.h"
#include "message_data.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <PN532_debug.h>


// NFC Declaration
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

// Display Declaration
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//NFC global variable
uint8_t        _prevIDm[8];
unsigned long  _prevTime;

//Useful Fuction
String convByteToString(const uint8_t* data, uint8_t length)
{
  String return_value;
  for(int i=0; i< length; i++)
  {
    if (data[i] < 0x10) 
    {
      return_value+="0"+String(data[i], HEX);
    } 
    else 
    {
      return_value+=String(data[i], HEX);
    }
  }
  return return_value;
}

void setup(void)
{
//Enable Required Hardware module and show welcome message

#if defined(SERIAL_OUTPUT)
//Enable USB Serial
  Serial.begin(115200);
  Serial.println(WORD_NFC_WELLCOME);
#endif

//Enable HD44780 LCD Display
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print(WORD_NFC_WELLCOME);
  lcd.backlight();

//Start the NFC Module
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) // If no nfc module detected
  {
#if defined(SERIAL_OUTPUT)
    Serial.print(WORD_NFC_INI_ERROR);
#endif

//HD44780 LCD Display
    lcd.setCursor(0, 10);
    lcd.print(WORD_NFC_INI_ERROR);
  }
//Print NFC Module data
  //Prepare data string
  String nfc_type = String(WORD_NFC_FOUND) + String(((versiondata >> 24) & 0xFF), HEX);
  String nfc_firmware = String(WORD_NFC_SHOW_FIRMWARE) + String(((versiondata >> 16) & 0xFF), DEC) + String(".") + String(((versiondata >> 8) & 0xFF), DEC);

#if defined(SERIAL_OUTPUT)
  // Got ok data, print it out!
  Serial.println(nfc_type);
  Serial.println(nfc_firmware);
#endif

  lcd.setCursor(0, 1);
  lcd.print(nfc_type);
  lcd.setCursor(0, 2);
  lcd.print(nfc_firmware);

  

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  memset(_prevIDm, 0, 8); //Clear the variable

  //Clean the display
  lcd.clear();
}

void loop(void)
{
  uint8_t ret;
  uint16_t systemCode = OCTOPUS_SYSTEM_CODE;
  uint8_t requestCode = OCTOPUS_REQUEST_CODE;       // System Code request
  uint8_t idm[8];
  uint8_t pmm[8];
  uint16_t systemCodeResponse;

  
#if defined(SERIAL_OUTPUT)
//Enable USB Serial
  Serial.print(WORD_NFC_WAIT_CARD);
#endif

  lcd.setCursor(0, 0);
  lcd.print(WORD_NULL_LINE); //Clean the line
  lcd.setCursor(0, 0);
  lcd.print(WORD_NFC_WAIT_CARD);


  // Poll the Octopus card
  ret = nfc.felica_Polling(systemCode, requestCode, idm, pmm, &systemCodeResponse, 5000);  

  //Cannot find an Octopus Card
  if (ret != 1)
  {
#if defined(SERIAL_OUTPUT)
    Serial.println(WORD_FELICA_NFOUND);
#endif
    delay(500);
    return;
  }
  // Found an Octopus Card
  else
  {
    //Find Same card id
    if ( memcmp(idm, _prevIDm, 8) == 0 ) 
    {
        //Not yet timeout, then skip processing
        if ( (millis() - _prevTime) < OCTOPUS_SAME_CARD_TIMEOUT ) 
        {
#if defined(SERIAL_OUTPUT)
        Serial.println(WORD_FELICA_SAME);
#endif
        delay(500);
        return;
        }
    }
    

//Found a new card
//Censor mode
#ifdef CENSOR_MODE
    String felica_idm = WORD_CARD_CENSOR;
    String felica_pmm = WORD_CARD_CENSOR;
//Not censor card data
#else
    String felica_idm = convByteToString(idm, 8);
    String felica_pmm = convByteToString(pmm, 8);
#endif
    String scode_re = String(systemCodeResponse, HEX);

// display the card info
#if defined(SERIAL_OUTPUT)
    Serial.println(WORD_FELICA_FOUND);
    Serial.print(WORD_FELICA_CARD_IDM);
    Serial.println(felica_idm);
    Serial.print(WORD_FELICA_CARD_PMM);
    Serial.println(felica_pmm);
#endif

  lcd.setCursor(0, 2);
  lcd.print(WORD_NULL_LINE); //Clean the line
  lcd.setCursor(0, 3);
  lcd.print(WORD_NULL_LINE); //Clean the line
  lcd.setCursor(0, 2);
  lcd.print(String(WORD_FELICA_CARD_IDM)+felica_idm); //Print Card ID
  lcd.setCursor(0, 3);
  lcd.print(String(WORD_FELICA_CARD_PMM)+felica_pmm); //Print Card PM



#if defined(SERIAL_OUTPUT)
    Serial.print(WORD_FELICA_CARD_SCODE);
    Serial.println(scode_re);
#endif
  

//Start Read the octopus card balance
    uint16_t serviceCodeList[1];
    uint16_t returnKey[1];
    uint16_t blockList[1];
    uint8_t blockData[1][16];


    serviceCodeList[0]=OCTOPUS_BALANCE_SERVICE_CODE;
    blockList[0] = OCTOPUS_BALANCE_SERVICE_BLOCK;


    //Request the Service, Read Balance
    // if Omit Request procedure, octopus on Apple Pay cannot complete the reading process until Timeout
    ret = nfc.felica_RequestService(1, serviceCodeList, returnKey); //Request the [fix the ]
    if(ret == 1)
    {
#if defined(SERIAL_OUTPUT)
        Serial.println(WORD_SUCESS_REQUEST);
#endif
    }
    //Fail to request Service
    else
    {
#if defined(SERIAL_OUTPUT)
      Serial.println(WORD_ERR_REQUEST);
#endif
      //LCD Jobs
            lcd.setCursor(0, 1);
            lcd.print(WORD_NULL_LINE); //Clean the line
            lcd.setCursor(0, 1);
            lcd.print(WORD_ERR_REQUEST);
    }

    //Finish Reading, Release the card    
    ret = nfc.felica_Release(); 
    if(ret == 1)
    {
#if defined(SERIAL_OUTPUT)
      Serial.println(WORD_SUCESS_RELEASE_CARD);
#endif
    }
    else
    {
#if defined(SERIAL_OUTPUT)
      Serial.println(WORD_FAIL_RELEASE_CARD);
#endif
    }

    // Wait 1 second before continuing
    memcpy(_prevIDm, idm, 8);
    _prevTime = millis();
#if defined(SERIAL_OUTPUT)
    Serial.println(WORD_PROCEDURE_FINISH);
#endif
    delay(1000);
  }
}