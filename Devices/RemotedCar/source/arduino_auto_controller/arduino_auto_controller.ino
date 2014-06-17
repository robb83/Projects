#include "Consts.h"
#include "Motor.h"
#include "Car.h"
#include "GP2Y0A21.h"
#include "ObstacleAvoidance.h"

Car car;
GP2Y0A21 leftSensor;
GP2Y0A21 centerSensor;
GP2Y0A21 rightSensor;
ObstacleAvoidance automate;
int serial_connected = 0;
HardwareSerial *listener;

void Reset()
{
    automate.SetEnabled(DISABLED);       
    car.Stop(); 
    if (listener != NULL)
    {
      listener->println(COM_F);
    }
    listener = NULL;
    car.AttachListener(NULL); 
}

void DisplayMCU(HardwareSerial *serial, unsigned long int *currentTime)
{
  // time
  serial->print(COM_A);
  serial->println(*currentTime); 

  // cpu
  serial->print(COM_B);
  serial->println(F_CPU);  
}

void DoCommand(HardwareSerial *serial)
{
  int command = serial->read();

  if (command == 'a')
  {
    unsigned long currentTime = millis();
    DisplayMCU(serial, &currentTime);
    car.DisplayBody(serial, &currentTime);
    car.DisplayState(serial, &currentTime);
    car.DisplayMove(serial, &currentTime, 0, 0);
  } 
  else if (command == 'b')
  {
    car.Forward();
  } 
  else if (command == 'c')
  {
    car.Backward();
  } 
  else if (command == 'd')
  {
    car.Stop();
  } else if(command == 'e')
  {
    // dummy
  }
  else if (command == 'f')
  {
    car.TurnLeft();
  } 
  else if (command == 'g')
  {
    car.TurnRight();
  } 
  else if (command == 'h')
  {
    car.Forward(serial->parseInt());
  } 
  else if (command == 'i')
  {
    car.Backward(serial->parseInt());
  } 
  else if (command == 'j')
  {
    car.TurnLeft(serial->parseInt());
  } 
  else if (command == 'k')
  {
    car.TurnRight(serial->parseInt());
  } else if (command == 'l')
  {
    listener = serial;
    car.AttachListener(serial);
  } else if (command == 'm')
  {
    listener = NULL;
    car.AttachListener(NULL);
  } else if (command == 'n')
  {
    Reset();
  } else if (command == 'o')
  {
    automate.SetEnabled(serial->parseInt());
  }
}

void setup() 
{
  Serial.begin(57600); // host pc
  Serial2.begin(38400); // bluetooth
  
  car.Init();
  leftSensor.Init(SENSOR_IR2);
  centerSensor.Init(SENSOR_IR1);
  rightSensor.Init(SENSOR_IR0);
  
  automate.Init(&car, &leftSensor, &centerSensor, &rightSensor);
}

void loop() 
{
  static unsigned long currentTime = 0;
  static unsigned long last_measurment = 0;
  static unsigned long last_received_time = 0;
  
  if (Serial.available())
  {
    DoCommand(&Serial);
    serial_connected = true;    
    last_received_time = millis();
  }

  if (Serial2.available())
  {
    DoCommand(&Serial2);
    serial_connected = true;
    last_received_time = millis();
  }
  
  currentTime = millis();
  
  // disconnected? listening?
  if (serial_connected)
  {
    if (currentTime - last_received_time > 5000)
    {
      serial_connected = false;
      Reset();
    }
  }

  car.Tick(currentTime);
  automate.Tick(&currentTime);
}

