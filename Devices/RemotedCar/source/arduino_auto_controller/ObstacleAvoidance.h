#pragma once

#include <Arduino.h>
#include "Consts.h"
#include "Car.h"
#include "GP2Y0A21.h"

class ObstacleAvoidance
{
  private:
    Car *m_car;
    GP2Y0A21 *m_left;
    GP2Y0A21 *m_center;
    GP2Y0A21 *m_right;
    int m_enabled;    
    int m_last_direction;

    boolean freeWayAhead()
    {
      float right = (m_right->ReadCentiMeter() + m_right->ReadCentiMeter() + m_right->ReadCentiMeter()) / 3;
      float center = (m_center->ReadCentiMeter() + m_center->ReadCentiMeter() + m_center->ReadCentiMeter()) / 3;
      float left = (m_left->ReadCentiMeter() + m_left->ReadCentiMeter() + m_left->ReadCentiMeter()) / 3;
    
      boolean freeLeft = left > OA_SIDE_THOLD;
      boolean freeRight = right > OA_SIDE_THOLD;
      boolean freeCenter = center > OA_AHEAD_THOLD;
      
      return freeCenter && freeLeft && freeRight;
    }

    int wayfinder()
    {
      float right = (m_right->ReadCentiMeter() + m_right->ReadCentiMeter() + m_right->ReadCentiMeter()) / 3;
      float center = (m_center->ReadCentiMeter() + m_center->ReadCentiMeter() + m_center->ReadCentiMeter()) / 3;
      float left = (m_left->ReadCentiMeter() + m_left->ReadCentiMeter() + m_left->ReadCentiMeter()) / 3;
      
      boolean barrierLeft = left < OA_SIDE_THOLD; 
      boolean barrierRight = right < OA_SIDE_THOLD;
      boolean barrierCenter = center < OA_AHEAD_THOLD;
      
      if (barrierCenter == false && barrierLeft == false && barrierRight == false) return DIRECTION_AHEAD;
      
      if ( right > left ) 
      {
        return DIRECTION_RIGHT; 
      }
      
      return DIRECTION_LEFT;
    }    

  public:
    ObstacleAvoidance()
    {
      m_last_direction = 0;
      m_enabled = DISABLED;
    }
    
    void Init(Car *car, GP2Y0A21 *left, GP2Y0A21 *center, GP2Y0A21 *right)
    {
      m_car = car;
      m_left = left;
      m_center = center;
      m_right = right;
    }
    
    int IsEnabled()
    {
      return m_enabled;
    }
    
    int SetEnabled(int value)
    {
      if (!value && m_enabled)
      {
        m_car->Stop();
      }
      
      m_enabled = value;
            
      m_last_direction = 0;  
    }
    
    void Tick(unsigned long int *currentMillisec)
    {      
      static unsigned long last_received_time = 0;
  
      if (!m_enabled) return;
      
      if ((*currentMillisec - last_received_time) < 50) return;
      last_received_time = *currentMillisec;
      
      if (freeWayAhead())
      {
        m_car->Forward();
      } 
      else 
      {
        int dir;
        
        if (m_last_direction == 0)
        {
          dir = wayfinder();
        }
        else if (freeWayAhead())
        {
          dir = 0;
        } else 
        {
          dir = m_last_direction;
        }
        
        if (dir == DIRECTION_RIGHT)
        {
          m_car->TurnRight();
        } else if (dir == DIRECTION_LEFT)
        {
          m_car->TurnLeft();
        }
        
        m_last_direction = dir;
      }
    }
};
