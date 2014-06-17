#pragma once
#include <Arduino.h>

/* http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/ */

class PID {
  private:
    double errSum, lastErr;
    double kp, ki, kd;
    double lastTime;
    
  public:
    PID()
    {
      kp = 1;
      ki = 1;
      kd = 1;      
    }
    
    PID(double kp, double ki, double kd) 
    {
      this->kp = kp;
      this->ki = ki;
      this->kd = kd;
      this->lastTime = 0;
    }
    
    double Compute(double Input, double Setpoint)
    {
       /*How long since we last calculated*/
       unsigned long now = millis();
       double timeChange = (double)(now - lastTime);
      
       /*Compute all the working error variables*/
       double error = Setpoint - Input;
       errSum += (error * timeChange);
       double dErr = (error - lastErr) / timeChange;
      
       /*Remember some variables for next time*/
       lastErr = error;
       lastTime = now;

       /*Compute PID Output*/       
       return (kp * error + ki * errSum + kd * dErr);
    }
};
