
#include <Wire.h>
#include "kxnI2C_ArmBuffer.h"

struct_I2C_ArmBuffer rcData;

void setup() {
  Wire.begin(); // join I2C bus (address optional for master)
  Serial.begin(9600);
  Serial.println("master slave");
}

void loop() {

  writeToArm(2000, 90, 91, 92, 0, 0, 95);
  delay(2000);

  writeToArm(2000, 90, 91, 92, 180, 180, 95);
  delay(2000);


}

void writeToArm(uint16_t paTime, uint8_t paAngel1, uint8_t paAngel2, uint8_t paAngel3, uint8_t paAngel4, uint8_t paAngel5, uint8_t paAngel6)
{
    rcData.values[0] = paAngel1;
    rcData.values[1] = paAngel2;
    rcData.values[2] = paAngel3;
    rcData.values[3] = paAngel4;
    rcData.values[4] = paAngel5;
    rcData.values[5] = paAngel6;
    rcData.time = paTime;
    
    Wire.beginTransmission(0x2a);
    Wire.write((uint8_t)(rcData.time & 0xFF));
    Wire.write((uint8_t)((rcData.time >> 8) & 0xFF));

    Wire.write(rcData.values[0]);
    Wire.write(rcData.values[1]);
    Wire.write(rcData.values[2]);
    Wire.write(rcData.values[3]);
    Wire.write(rcData.values[4]);
    Wire.write(rcData.values[5]);
    Wire.endTransmission(); // stop transmitting
    
}


