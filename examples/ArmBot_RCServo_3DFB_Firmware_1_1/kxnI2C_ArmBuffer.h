#ifndef kxnI2C_ArmBuffer__H
#define kxnI2C_ArmBuffer__H

#include "Arduino.h"

typedef struct
{
    uint16_t time;
    uint8_t values[6];
} struct_I2C_ArmBuffer;


#endif