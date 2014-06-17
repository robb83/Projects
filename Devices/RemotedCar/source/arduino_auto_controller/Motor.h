#ifndef MOTOR001
#define MOTOR001 1
  
#include <Arduino.h>

class Motor {
  public: 
    int encoder_pin;
    int interrupt_pin;
    int speed;
    int direction;
    int control_pin; // E
    int direction_pin; // M
  
    Motor(int ep, int ip, int cp, int dp)
    {
      encoder_pin = ep;
      interrupt_pin = ip;
      speed = MOTOR_MIN_SPEED;
      direction = MOTOR_FORWARD;
      control_pin = cp;
      direction_pin = dp;
    }
    
    void Init()
    {
      // pinMode(direction_pin, OUTPUT);
      pinMode(control_pin, OUTPUT);
      
      SetDirection(direction);
      SetSpeed(speed);
    }
    
    void Stop()
    {
      direction = (direction == MOTOR_BACKWARD ? MOTOR_FORWARD : MOTOR_BACKWARD);
      digitalWrite(direction_pin, direction);
      delay(100);
      SetSpeed(MOTOR_MIN_SPEED);      
    }
    
    void SetSpeedAndDirection(int s, int d)
    {
      SetSpeed(s);
      SetDirection(d);
    }
    
    void SetSpeed(int value)
    {
      speed = constrain(value, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
      analogWrite(control_pin, speed);
    }
       
    void SetDirection(int value)
    {
      direction = (value == MOTOR_BACKWARD ? MOTOR_BACKWARD : MOTOR_FORWARD);
      digitalWrite(direction_pin, value);
    }
    
    void Reverse()
    {
      SetDirection((direction == MOTOR_FORWARD ? MOTOR_BACKWARD : MOTOR_FORWARD));      
    }
};

#endif

