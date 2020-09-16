#pragma once

#include <Arduino.h>

// uint64_t convBytesTouint64(const uint8_t* data)
// {
//     //Should be input 8 bytes data
//     uint64_t return_val = 0;

// }

String convBytesToString(const uint8_t* data, uint8_t length)
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