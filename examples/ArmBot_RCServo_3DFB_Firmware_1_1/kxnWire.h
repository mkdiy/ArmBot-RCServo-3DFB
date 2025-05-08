/*
    Ý tưởng:
        - 25/4/2025 
            - Nhận lệnh điều khiển từ I2C với Format là Chuỗi
              - VD: 1  100\n
                  - Điều khiển RC 1 góc 100

    Kết quả:
        - 25/4/2025
            - Việc gửi I2C liên tục sẽ làm RC Giựt Giựt
                * Chuẩn đoán có thể do ngắt I2C ảnh hưởng đến timer điều khiển RC
*/

#ifndef kxnWire__H
#define kxnWire__H

#include <Wire.h>
#include "kxnI2C_ArmBuffer.h"

struct_I2C_ArmBuffer rcData;

extern void SerialControl_I2C(Stream *pSerial);
void receiveEvent(int howMany); // function prototype

class kxnWire
{
  public:
    void setup(int paAddress)
    {
        Wire.begin(paAddress);                // join I2C bus with address #8
        Wire.onReceive(receiveEvent); // register event
    }
};

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) 
{
    SerialControl_I2C(&Wire);
}
#endif