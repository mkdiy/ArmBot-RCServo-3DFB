#ifndef kxnTask_RCservo__H
#define kxnTask_RCservo__H

#include "kxnTask.h"
#include "Servo.h"

#define _VIETDUINI_SERVO_VER        1.0
#define _VIETDUINO_SERVO_DATE       250426

#define _MAX_ANGLE_VALUE_       posMax
#define _MIN_ANGLE_VALUE_       posMin

// #define _MAX_ANGLE_VALUE_       180
// #define _MIN_ANGLE_VALUE_       0

#define _MAX_SPEED_VALUE_       9
#define _MIN_SPEED_VALUE_       1
#define SERVO_MAP_SPEED_X(x, y)         map(x,0,100,_MIN_SPEED_VALUE_,y)
#define SERVO_MAP_SPEED(x)              SERVO_MAP_SPEED_X(x,_MAX_SPEED_VALUE_)

extern "C" {
    typedef void (*ServoWrite_pf)(int pin, unsigned char _angel_);
}

CREATE_TASK_STATE(kxnTask_RCservo)

kxnTask_RCservo_RUN,

END


CREATE_TASK(kxnTask_RCservo)
    void setup(int pin);
    void setup(Servo * paServo);
    void setup(int pin, ServoWrite_pf _servowrite_f_);

    void setAngleMin(int pin) { posMin = pin; }
    void setAngleMax(int pin) { posMax = pin; }

    // ----- Set runtime parameters -----
    // static void tick(Vietduino_Task * me);
    void write(unsigned char _Angle_, unsigned char _speed_);
    void write(unsigned char _Angle_);
    void write_for(float _Angle_, unsigned long _time_in_ms_);
    virtual void Servo_write(unsigned char _Angle_);
    void attachWrite(ServoWrite_pf _servowrite_f_);

    // Return true when Servo stop
    bool available();

    void stop();
    uint8_t getCurrentAngle();

    void loop();

    void start()
    {
        kxnTaskManager.add(this);
        setState(kxnTask_RCservo_RUN);
    }

    void writeNewPos(float oldAnglePos, float newAnglePos, unsigned long maxTimeMs, float minStepAngle, unsigned long minStepTimeMs);
    private:
    Servo * myservo;
    int posNeed, pos, Servo_speed, lastpos;
    int posMax = 180, posMin = 0;
    unsigned long time_interval = 30;
    ServoWrite_pf  pf_serWrite;

    void checkValid_Angle(unsigned char _Angle_);
    uint8_t ui8_servoPin;
    uint8_t flagRunDoneDebug = false;
END
#endif