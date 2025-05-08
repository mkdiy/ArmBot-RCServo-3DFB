#include "kxnTask_RCservo.h"

// kxnTask_RCservo::kxnTask_RCservo(int _pin_):
//     Vietduino_Task(30, &kxnTask_RCservo::tick){

//     myservo = new(Servo);
//     myservo->attach(_pin_);
//     posNeed = 0;
//     pos = 0;
//     Servo_speed = _MIN_SPEED_VALUE_;
//     lastpos = 0;

//     Vietduino_Manager_3.add(this);
    
// }

// kxnTask_RCservo::~kxnTask_RCservo(){

// }

void kxnTask_RCservo::setup(int pin)
{
    myservo = new(Servo);
    myservo->attach(pin);
    posNeed = 90;
    pos = 90;
    Servo_speed = _MIN_SPEED_VALUE_;
    lastpos = 0;

    kxnTaskManager.add(this);
    setState(0);
    kDelay(time_interval);
}

void kxnTask_RCservo::setup(Servo * paServo)
{
    myservo = paServo;
    posNeed = 90;
    pos = 90;
    Servo_speed = _MIN_SPEED_VALUE_;
    lastpos = 0;

    kxnTaskManager.add(this);
    setState(0);
    kDelay(time_interval);
}

void kxnTask_RCservo::setup(int pin, ServoWrite_pf _servowrite_f_)
{
    ui8_servoPin = pin;
    pf_serWrite = _servowrite_f_;

    kxnTaskManager.add(this);
    setState(0);
    kDelay(time_interval);
}

void kxnTask_RCservo::write(unsigned char _Angle_, unsigned char _speed_){
    checkValid_Angle(_Angle_);

    if(SERVO_MAP_SPEED(_speed_) >= _MAX_SPEED_VALUE_){
        Servo_speed = _MAX_SPEED_VALUE_;
    }else if(SERVO_MAP_SPEED(_speed_) <= _MIN_SPEED_VALUE_){
        Servo_speed = _MIN_SPEED_VALUE_;
    }else{
        Servo_speed = SERVO_MAP_SPEED(_speed_);
    }
}

void kxnTask_RCservo::write(unsigned char _Angle_)
{
    write(_Angle_, 100);
}

void kxnTask_RCservo::write_for(float _Angle_, unsigned long _time_in_ms_)
{
    checkValid_Angle(_Angle_);
    writeNewPos(lastpos, posNeed, _time_in_ms_, 1, 30);
    flagRunDoneDebug = true;
    // Serial.println("Servo_speed: " + String(Servo_speed) + " StepTimeMs: " + String(interval) + " time: " + String(_time_in_ms_));
}

void kxnTask_RCservo::Servo_write(unsigned char _Angle_)
{
    if(this->myservo){
        this->myservo->write(_Angle_);
    } 

    if(this->pf_serWrite) this->pf_serWrite(ui8_servoPin, _Angle_);
}

void kxnTask_RCservo::attachWrite(ServoWrite_pf _servowrite_f_)
{
    pf_serWrite = _servowrite_f_;
}

bool kxnTask_RCservo::available()
{
    return (lastpos == posNeed);
}

void kxnTask_RCservo::stop()
{
    this->posNeed = this->pos;
}

uint8_t kxnTask_RCservo::getCurrentAngle()
{
    return this->pos;
}

void kxnTask_RCservo::checkValid_Angle(unsigned char _Angle_)
{
    if(_Angle_ >= _MAX_ANGLE_VALUE_){
        posNeed = _MAX_ANGLE_VALUE_;
    }else if(_Angle_ <= _MIN_ANGLE_VALUE_){
        posNeed = _MIN_ANGLE_VALUE_;
    }else{
        posNeed = _Angle_;
    }
}

void kxnTask_RCservo::writeNewPos(float oldAnglePos, float newAnglePos, unsigned long maxTimeMs, float minStepAngle, unsigned long minStepTimeMs) {
    unsigned long StepTimeMs = 0; // Thời gian bước tính toán
    float StepAngel = 0; // Góc bước tính toán
    unsigned long num_steps = 0; // Số bước tối ưu

    // 1. Tính khoảng cách góc tổng cần di chuyển.
    float totalAngle = abs(newAnglePos - oldAnglePos);

    // 2. Tính số bước tối thiểu dựa trên MinStepAngle.
    unsigned long minSteps = ceil(totalAngle / minStepAngle);

    // 3. Nếu số bước tối thiểu lớn hơn 1, tính toán thời gian bước dựa trên số bước tối thiểu.
    if (minSteps > 1) {
        float calculatedStepTimeMs = (float)maxTimeMs / minSteps;

        // 4. Kiểm tra nếu thời gian bước tính toán nhỏ hơn thời gian bước tối thiểu.
        if (calculatedStepTimeMs < minStepTimeMs) {
            StepTimeMs = minStepTimeMs;
            // Tính lại số bước tối ưu để không vượt quá maxTimeMs
            num_steps =  ceil((float)maxTimeMs / StepTimeMs);
            StepAngel = totalAngle / num_steps;
        } else {
            // Nếu thời gian bước hợp lệ, sử dụng nó.
            StepTimeMs = calculatedStepTimeMs;
            StepAngel = totalAngle / minSteps;
            num_steps = minSteps;
        }
    }
    else // trường hợp góc di chuyển nhỏ hơn minStepAngle
    {
        StepTimeMs = maxTimeMs;
        StepAngel = totalAngle;
        num_steps = 1;
    }
    

    // // In kết quả ra Serial để debug (có thể bỏ sau khi kiểm tra).
    // Serial.print("Old Angle: ");       Serial.println(oldAnglePos);
    // Serial.print("New Angle: ");       Serial.println(newAnglePos);
    // Serial.print("Max Time (ms): ");    Serial.println(maxTimeMs);
    // Serial.print("Min Step Angle: ");  Serial.println(minStepAngle);
    // Serial.print("Min Step Time (ms): "); Serial.println(minStepTimeMs);
    // Serial.print("Step Angle: ");      Serial.println(StepAngel);
    // Serial.print("Step Time (ms): ");     Serial.println(StepTimeMs);
    // Serial.print("Number of Steps: ");     Serial.println(num_steps);
    
    // // Lưu các biến tính toán được vào biến thành viên của lớp.
    // this->angle_step = StepAngel; // biến của lớp
    // this->time_interval = StepTimeMs; // biến của lớp.

    kDelay(StepTimeMs); // Đợi thời gian bước trước khi tiếp tục
    posNeed = newAnglePos; // Cập nhật góc cần di chuyển mới
    Servo_speed = StepAngel + 0.5;
}

void kxnTask_RCservo::loop()
{
    // kxnTask_RCservo *tem = (kxnTask_RCservo*) me;
    kxnTask_RCservo *tem = this;
    
    if(tem->posNeed > tem->pos){
      tem->pos += tem->Servo_speed;
      if(tem->pos > tem->posNeed){
        // if((tem->pos > tem->posNeed) || (tem->Servo_speed == _MAX_SPEED_VALUE_)){
        tem->pos = tem->posNeed;
      }
    }else if(tem->posNeed < tem->pos){
      tem->pos -= tem->Servo_speed;
      if(tem->pos < tem->posNeed){
        // if((tem->pos < tem->posNeed) || (tem->Servo_speed == _MAX_SPEED_VALUE_)){
        tem->pos = tem->posNeed;
      }
    }else
    {
        if(flagRunDoneDebug == true)
        {
            flagRunDoneDebug = false;
            Serial.println("Servo " + String(tem->ui8_servoPin) + " Done!");
        }
    }

    if(tem->lastpos != tem->pos){
      tem->Servo_write(tem->pos);
      tem->lastpos = tem->pos;
    //   kDelay(0);
    
    //   Serial.println(String() +"S" + tem->ui8_servoPin + " " + tem->pos);
    }
    else
    {
        // if(flagRunDoneDebug == true)
        // {
        //     flagRunDoneDebug = false;
        //     Serial.println("Servo " + String(tem->ui8_servoPin) + " Done!");
        // }
    }
    
}