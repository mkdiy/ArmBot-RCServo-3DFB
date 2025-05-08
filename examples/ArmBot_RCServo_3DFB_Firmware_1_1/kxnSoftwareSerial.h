#ifndef kxnSoftwareSerial__H
#define kxnSoftwareSerial__H
#include "SoftwareSerial.h"

extern void RunRC(int servoIndex, int servoValue);

class kxnSoftwareSerial
{
  public:
  SoftwareSerial * pSerial;
  void setup()
  {
    if(pSerial == NULL)
    {
      pSerial = new SoftwareSerial(2, 3); // RX, TX
      pSerial->begin(9600);
    }
    else
    {
      pSerial->begin(9600);
    }
  }

  // void loop()
  // {
  //   if (pSerial->available()) {
  //     String input = pSerial->readStringUntil('\n'); // Read the data string until newline
  //     int servoIndex = input.substring(0, 1).toInt(); // Get the servo index
  //     int servoValue = input.substring(2).toInt(); // Get the servo value
  //     RunRC(servoIndex, servoValue);
  //   }
  // }
};
#endif