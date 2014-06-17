#pragma once

#include <Arduino.h>
#include "Consts.h"

class GP2Y0A21
{
  private:
    int m_analog_pin;
    int m_measurements;
    int m_last_value;

  public:
    GP2Y0A21()
    {
      m_measurements = 0;
      m_last_value = 0;
    }
    
    void Init(int analog_pin)
    {
      m_analog_pin = analog_pin;
    }
    
    int Read()
    {
      return analogRead(m_analog_pin);
    }
    
    float ReadCentiMeter()
    {
      m_last_value = Read();
      m_measurements++;

      if (m_last_value < 0) return GP2Y0A21_raw_to_cm[0];
      if (m_last_value >= 650) return GP2Y0A21_raw_to_cm[649];
    
      return GP2Y0A21_raw_to_cm[m_last_value];
    }
};
