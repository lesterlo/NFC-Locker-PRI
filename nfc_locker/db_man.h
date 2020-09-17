#pragma once 


#define PROGRAM_CONF_START_ADDR 0
#define CARD_RECORD_START_ADDR  20

#define MAX_STORE_ENTRY 10

#include <Arduino.h>
#include <EEPROM.h>

union card_id_t
{
  uint8_t b8a[8];
  uint64_t b64; 
};

struct prog_conf{
  int num_active_record;

};

struct card_obj{
  card_id_t card_id;
  char name[10];
};


class DB_man
{
public:
    //Constructor
    DB_man
    (           
    );
    //Destructor
    ~DB_man();
    void init();
    int sync_db();
    int getCard(int, card_obj&); //Get from memory
    int findCard(card_id_t );
    int read_eeprom_entry(int, card_obj&);//Read the eeprom
    int write_entry(int, const card_obj&);
    int read_all_entry(card_obj []);
 
private:
    card_obj _card_table[MAX_STORE_ENTRY];
};
