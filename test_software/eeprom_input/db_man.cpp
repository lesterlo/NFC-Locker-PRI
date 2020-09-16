#include "db_man.h"

DB_man::DB_man()
{

}

DB_man::~DB_man()
{
    
}



void DB_man::init()
{
    //Read all card entry from the EEPROM

}

int DB_man::read_entry(int index, card_obj &cobj)
{
    if((index >= 0) && (index < MAX_STORE_ENTRY))
    {
        int read_index = 0;

        EEPROM.get(CARD_RECORD_START_ADDR+index*sizeof(card_obj), cobj);
        return 0; //Read eeprom complete
    }
    else
    {
        return -1; //Wrong position index, return error
    }
}

int DB_man::write_entry(int index, const card_obj &cobj)
{
    if((index >= 0) && (index < MAX_STORE_ENTRY))
    {
        int read_index = 0;

        EEPROM.put(CARD_RECORD_START_ADDR+index*sizeof(card_obj), cobj);
        return 0; //Read eeprom complete
    }
    else
    {
        return -1; //Wrong position index, return error
    }
}

int DB_man::read_all_entry(card_obj cobj_arr[])
{
    for(int i=0; i < MAX_STORE_ENTRY; i++)
    {

    }
}