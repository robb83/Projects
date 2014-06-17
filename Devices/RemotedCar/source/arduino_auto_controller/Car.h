#pragma once
#include <Arduino.h>
#include "PID.h"

class Car {
  private:
    Motor *m_leftMotor;
    Motor *m_rightMotor;
    HardwareSerial *m_listener;
    unsigned long int m_listener_timeout;
    unsigned long int m_position_timeout;
    float m_x;
    float m_y;
    float m_heading;
    int m_state;
    int m_previous_state;
    int m_previous_moved_ticks;
    int m_pid_control_enabled;
    double m_move_distance;
    PID *m_right_motor_pid;
    static int s_left_ticks;
    static int s_right_ticks;
    static int s_left_ticks_distance;
    static int s_right_ticks_distance;
    static int s_left_ticks_position;
    static int s_right_ticks_position;
    static int s_left_ticks_display;
    static int s_right_ticks_display;
    static void LeftCounter() { ++s_left_ticks; }
    static void RightCounter() { ++s_right_ticks; }
    
    void SetMoveState(int distanceInTicks)
    {
      s_left_ticks_distance = s_left_ticks;
      s_right_ticks_distance = s_right_ticks;
      m_move_distance = distanceInTicks - CAR_BREAK_MOMENT;
      m_previous_state = m_state;
      m_state = CAR_STATE_MOVE;
    }
    
  public:
    Car() {
      m_listener = NULL;
      m_pid_control_enabled = ENABLED;
      m_listener_timeout = 0;
      m_move_distance = 0;
      m_state = CAR_STATE_UNINITIALIZED;
      m_previous_state = CAR_STATE_UNINITIALIZED;
      m_previous_moved_ticks = 0;
      m_heading = 0;
      m_x = 0;
      m_y = 0;      
      m_leftMotor = new Motor(MOTOR_P1, MOTOR_I1, MOTOR_E1, MOTOR_M1);
      m_rightMotor = new Motor(MOTOR_P2, MOTOR_I2, MOTOR_E2, MOTOR_M2);
      m_right_motor_pid = new PID(1000, 1 * (CAR_POSITION_TIMEOUT / 1000) , 2 / (CAR_POSITION_TIMEOUT / 1000));
    }
    
    int GetState() { return m_state; }
    float GetHeading() { return m_heading; }
    
    void Init() {
      if (m_state == CAR_STATE_UNINITIALIZED)
      {
        m_leftMotor->Init();
        m_rightMotor->Init();
        
        // ticks counter 
        attachInterrupt(m_leftMotor->interrupt_pin, &Car::LeftCounter, CHANGE);
        attachInterrupt(m_rightMotor->interrupt_pin, &Car::RightCounter, CHANGE);
        
        m_previous_state = m_state;
        m_state = CAR_STATE_NONE;
      }
    }
    
    void AttachListener(HardwareSerial *serial)
    {
      m_listener = serial;
    }
    
    void Tick(unsigned long int current_millisec)
    {        
      if (m_state == CAR_STATE_MOVE)
      {
        int ltd = s_left_ticks;
        int rtd = s_right_ticks;
        
        m_move_distance -= ((double)(ltd - s_left_ticks_distance) + (rtd - s_right_ticks_distance)) / 2.0;
        
        s_left_ticks_distance = ltd;
        s_right_ticks_distance = rtd;
                
        if (m_move_distance > 0 && m_move_distance <= CAR_BREAK_LENGTH)
        {
          m_leftMotor->SetSpeed(MOTOR_SLW_SPEED);
          m_rightMotor->SetSpeed(MOTOR_SLW_SPEED);
        } else if (m_move_distance <= 0)
        {
          m_move_distance = 0;
          Stop();
        }
      }
      
      if (m_position_timeout <= current_millisec)
      {
        m_position_timeout = current_millisec + CAR_POSITION_TIMEOUT;
        
        int left_ticks = s_left_ticks;
        int right_ticks = s_right_ticks;

        float leftDistance = (float)(left_ticks - s_left_ticks_position) * (float)WHEEL_DISTANCE_PER_TICKS * (m_leftMotor->direction == 0 ? (float)1 : (float)-1 );
        float rightDistance = (float)(right_ticks - s_right_ticks_position) * (float)WHEEL_DISTANCE_PER_TICKS * (m_rightMotor->direction == 0 ? (float)1 : (float)-1 );
        float distance = ((float)rightDistance + (float)leftDistance) / 2;
        float offsetHeading = ((float)rightDistance - (float)leftDistance) / ((float)AXIS_WIDTH * 2); // ((float)AXIS_WIDTH / 2)
        
        m_heading = m_heading + offsetHeading;
        NORMALRAD(m_heading);
        
        m_x += distance * cos(m_heading);
        m_y += distance * sin(m_heading); 
      
        if (m_pid_control_enabled == ENABLED && m_state == CAR_STATE_MOVE)
        {         
          // PID
          
          double force = m_right_motor_pid->Compute(
            (right_ticks - s_right_ticks_position), 
            (left_ticks - s_left_ticks_position)
          );
          /*
          if (m_listener != NULL)
          {
            m_listener->print((right_ticks - s_right_ticks_position));
            m_listener->print(" : ");
            m_listener->print((left_ticks - s_left_ticks_position));
            m_listener->print(" : ");          
            m_listener->println(force);
          }*/
          
          if (force < 0)
          {
            if (m_leftMotor->speed >= MOTOR_MAX_SPEED)
            {
              m_rightMotor->SetSpeed(constrain(m_rightMotor->speed - abs(force), MOTOR_MID_SPEED, MOTOR_MAX_SPEED));
            } 
            else {
              m_leftMotor->SetSpeed(m_leftMotor->speed + abs(force));
            }
          } else if (force > 0) {
            if (m_rightMotor->speed >= MOTOR_MAX_SPEED)
            {
              m_leftMotor->SetSpeed(constrain(m_leftMotor->speed - abs(force), MOTOR_MID_SPEED, MOTOR_MAX_SPEED));
            } 
            else {
              m_rightMotor->SetSpeed(m_rightMotor->speed + abs(force)); 
            }
          }
        }
      
        s_left_ticks_position = left_ticks;
        s_right_ticks_position = right_ticks;                        
      }
           
      if (m_state != m_previous_state)
      {
        m_previous_state = m_state;
        if (m_listener != NULL)
        {
          DisplayState(m_listener, &current_millisec);
        }
      }
      
      if (m_listener_timeout <= current_millisec)
      {
        int dltd = s_left_ticks;
        int drtd = s_right_ticks;
        
        int ldiff = (dltd - s_left_ticks_display);
        int rdiff = (drtd - s_right_ticks_display);    
        
        s_left_ticks_display = dltd;
        s_right_ticks_display = drtd;
        
        if (ldiff != 0 || rdiff != 0)
        {
          m_listener_timeout = current_millisec + CAR_DISPLAY_TIMEOUT;
          
          if (m_listener != NULL)
          {
            DisplayMove(m_listener, &current_millisec, ldiff, rdiff);            
          }
        }
      }      
    }
    
    void Forward() {
      m_leftMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_LEFT_SYNC, MOTOR_FORWARD); 
      m_rightMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_RIGHT_SYNC, MOTOR_FORWARD);     
    }
    
    void Forward(int distance) {
      if (m_state != CAR_STATE_NONE) return;
            
      SetMoveState(((distance * 10) / WHEEL_DISTANCE_PER_TICKS));
      
      Forward();
    }
    
    void Backward() {
      m_leftMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_LEFT_SYNC, MOTOR_BACKWARD); 
      m_rightMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_RIGHT_SYNC, MOTOR_BACKWARD);
    }
    
    void Backward(int distance) {
      if (m_state != CAR_STATE_NONE) return;
      
      SetMoveState(((distance * 10) / WHEEL_DISTANCE_PER_TICKS));
      
      Backward();
    }
    
    void TurnLeft() {
      m_leftMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_LEFT_SYNC, MOTOR_BACKWARD); 
      m_rightMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_RIGHT_SYNC, MOTOR_FORWARD);    
    }
    
    void TurnLeft(int angle) {
      if (m_state != CAR_STATE_NONE) return;
      
      SetMoveState((radians(angle) * AXIS_WIDTH) / WHEEL_DISTANCE_PER_TICKS);
      
      TurnLeft();
    }
    
    void TurnRight() {
      m_leftMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_LEFT_SYNC, MOTOR_FORWARD); 
      m_rightMotor->SetSpeedAndDirection(MOTOR_MAX_SPEED * MOTOR_RIGHT_SYNC, MOTOR_BACKWARD);
    }
    
    void TurnRight(int angle) {
      if (m_state != CAR_STATE_NONE) return;
      
      SetMoveState((radians(angle) * AXIS_WIDTH) / WHEEL_DISTANCE_PER_TICKS);
      
      TurnRight();      
    }
    
    void Stop() {
      m_leftMotor->Reverse();
      m_rightMotor->Reverse();
      delay(100);
      m_leftMotor->SetSpeed(MOTOR_MIN_SPEED);
      m_rightMotor->SetSpeed(MOTOR_MIN_SPEED);
      
      m_previous_state = m_state;
      m_state = CAR_STATE_NONE;
    }
   
    void DisplayBody(HardwareSerial *serial, unsigned long int *currentTime)
    {
      // physical details
      serial->print(COM_C);
      serial->print(WHEEL_DIAMETER);
      serial->print(":");
      serial->print(WHEEL_WIDTH);
      serial->print(":");
      serial->print(ENCODER_TICKS_PER_REVOLUTION);
      serial->print(":");
      serial->print(AXIS_WIDTH);
      serial->print(":");
      serial->print(AXIS_DISTANCE);
      serial->print(":");
      serial->print(BODY_WIDTH);
      serial->print(":");
      serial->println(BODY_HEIGHT);
    }
  
    void DisplayState(HardwareSerial *serial, unsigned long int *currentTime)
    {
      serial->print(COM_D);
      serial->print(m_state);
      serial->print(":");
      serial->println(*currentTime);
    }
  
    void DisplayMove(HardwareSerial *serial, unsigned long int *currentTime, int leftticks, int rightticks)
    {
      serial->print(COM_E);
      serial->print(leftticks);
      serial->print(":");
      serial->print(rightticks);      
      serial->print(":");      
      serial->print(m_leftMotor->speed);
      serial->print(":");
      serial->print(m_rightMotor->speed);      
      serial->print(":");
      serial->print(m_leftMotor->direction);      
      serial->print(":");
      serial->print(m_rightMotor->direction);      
      serial->print(":");
      serial->print(m_x);
      serial->print(":");
      serial->print(m_y);
      serial->print(":");
      serial->print(degrees(m_heading));
      serial->print(":");
      serial->println(*currentTime);
    }
};

int Car::s_left_ticks = 0;
int Car::s_right_ticks = 0;
int Car::s_left_ticks_display = 0;
int Car::s_right_ticks_display = 0;
int Car::s_left_ticks_distance = 0;
int Car::s_right_ticks_distance = 0;
int Car::s_left_ticks_position = 0;
int Car::s_right_ticks_position = 0;
