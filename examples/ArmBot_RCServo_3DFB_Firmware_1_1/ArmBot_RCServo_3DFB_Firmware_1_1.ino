#include "Servo.h"
#include "kxnSoftwareSerial.h"
#include "kxnWire.h"
#include "kxnTask_RCservo.h"

kxnTask_RCservo kxnTask_RCservo0;
kxnTask_RCservo kxnTask_RCservo1;
kxnTask_RCservo kxnTask_RCservo2;
kxnTask_RCservo kxnTask_RCservo3;
kxnTask_RCservo kxnTask_RCservo4;
kxnTask_RCservo kxnTask_RCservo5;
kxnTask_RCservo kxnTask_RCservo6;
kxnSoftwareSerial sSerial;
kxnWire kxnWire1;
Servo servo_0; // Declaration of object to control the first servo
Servo servo_1; // Declaration of object to control the second servo
Servo servo_2; // Declaration of object to control the third servo
Servo servo_3; // Declaration of object to control the fourth servo
Servo servo_4; // Declaration of object to control the fifth servo
Servo servo_5; // Declaration of object to control the sixth servo
Servo servo_6; // Declaration of object to control the seventh servo (not used in this project)

void setup() {
  servo_0.attach(12); // Associate servo_0 to pin 2
  servo_1.attach(11); // Associate servo_1 to pin 3
  servo_2.attach(10); // Associate servo_2 to pin 4
  servo_3.attach(9); // Associate servo_3 to pin 5
  servo_4.attach(8); // Associate servo_4 to pin 6
  servo_5.attach(7); // Associate servo_5 to pin 7
  servo_6.attach(6); // Associate servo_6 to pin 8

  kxnTask_RCservo0.setup(&servo_0); // Initialize the servo task

  kxnTask_RCservo0.setAngleMin(90);
  kxnTask_RCservo0.setAngleMax(150);

  kxnTask_RCservo1.setup(&servo_1); // Initialize the servo task
  kxnTask_RCservo2.setup(&servo_2); // Initialize the servo task

  kxnTask_RCservo3.setup(&servo_3); // Initialize the servo task
  kxnTask_RCservo4.setup(&servo_4); // Initialize the servo task
  kxnTask_RCservo5.setup(&servo_5); // Initialize the servo task
  kxnTask_RCservo6.setup(&servo_6); // Initialize the servo task

  Serial.begin(9600); // Initialize serial communication
  sSerial.setup();

  kxnWire1.setup(0x2a); // Initialize I2C communication
}

void loop() {

  SerialControl(&Serial);
  SerialControl(sSerial.pSerial);
  kxnTaskManager.run(millis()); // Run the task manager to handle tasks
}

void SerialControl(Stream *pSerial)
{
  while (pSerial->available() > 0) { // If there is data available to read
    String input = pSerial->readStringUntil('\n'); // Read the data string until newline
    int servoIndex = input.substring(0, 1).toInt(); // Get the servo index
    int servoValue = input.substring(2).toInt(); // Get the servo value
    
    RunRC(servoIndex, servoValue);
  }
}

void SerialControl_I2C(Stream *pSerial)
{
  SerialControl_Buffer(pSerial); // Call the function to handle the buffer data
}

void SerialControl_Buffer(Stream *pSerial)
{
  uint8_t * pTempValue = (uint8_t *) &rcData;

  for (int cf = 0; cf < sizeof(struct_I2C_ArmBuffer); cf++)
  {
    if(pSerial->available() == 0) break; // If there is no data available to read
    *pTempValue = pSerial->read();
    pTempValue++;
  }

  kxnTask_RCservo0.write_for(180 - rcData.values[0], rcData.time); // Move servo_0 to the specified angle over 1 second
  kxnTask_RCservo1.write_for(rcData.values[1], rcData.time); // Move servo_1 to the specified angle over 1 second
  kxnTask_RCservo2.write_for(rcData.values[2], rcData.time); // Move servo_2 to the specified angle over 1 second
  kxnTask_RCservo3.write_for(rcData.values[3], rcData.time); // Move servo_3 to the specified angle over 1 second
  kxnTask_RCservo4.write_for(rcData.values[4], rcData.time); // Move servo_4 to the specified angle over 1 second
  kxnTask_RCservo5.write_for(rcData.values[5], rcData.time); // Move servo_5 to the specified angle over 1 second
  kxnTask_RCservo6.write_for(180 - rcData.values[4], rcData.time); // Move servo_6 to the specified angle over 1 second
  
}

void RunRC(int servoIndex, int servoValue)
{
  switch (servoIndex) {
      case 1:
        kxnTask_RCservo0.write(180 - servoValue);
        break;

      case 2:
        kxnTask_RCservo1.write(servoValue);
        break;

      case 3:
        kxnTask_RCservo2.write(servoValue);
        break;

      case 4:
        kxnTask_RCservo3.write(servoValue);
        break;

      case 5:
        kxnTask_RCservo4.write(servoValue);
        kxnTask_RCservo6.write(180 - servoValue);
        break;

      case 6:
        kxnTask_RCservo5.write(servoValue);
        break;

      default:
        // Invalid servo index
        break;
    }
}

