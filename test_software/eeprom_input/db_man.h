#pragma once 

#define MAX_STORE_ENTRY 10

#include <Arduino.h>
#include <EEPROM.h>

struct card_obj{
  uint64_t card_id;
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
    void read_all(card_obj []);
 
private:
    
};
