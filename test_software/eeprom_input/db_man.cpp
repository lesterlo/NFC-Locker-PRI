#include "db_man.h"

DB_man::DB_man()
{
    sync_db();
}

DB_man::~DB_man()
{
    
}



void DB_man::init()
{
    //Read all card entry from the EEPROM

}

int DB_man::sync_db()
{
    for (int i = 0; i < MAX_STORE_ENTRY; i++)
    {
        card_obj temp; 
        read_eeprom_entry(i, temp);
        memcpy(_card_table+i*sizeof(card_obj), &temp, sizeof(card_obj));
    }
    return 0;
}

int DB_man::findCard(card_id_t in_card)
{
    for(int i=0; i < MAX_STORE_ENTRY; i++)
    {
        if(memcmp((_card_table+i*sizeof(card_obj))->card_id.b8a, in_card.b8a, 8*sizeof(uint8_t)) == 0)
            return i; //return the position   
    }
    return -1;
}

int DB_man::getCard(int index, card_obj &cobj)
{
    if((index >= 0) && (index < MAX_STORE_ENTRY))
    {
        memcpy(&cobj, _card_table+index*sizeof(card_obj), sizeof(card_obj));
        return 0; //Read cache complete
    }
    else
    {
        return -1; //Wrong position index, return error
    }
}

int DB_man::read_eeprom_entry(int index, card_obj &cobj)
{
    if((index >= 0) && (index < MAX_STORE_ENTRY))
    {
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
        EEPROM.put(CARD_RECORD_START_ADDR+index*sizeof(card_obj), cobj);

        sync_db();
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