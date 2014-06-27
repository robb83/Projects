#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#define BUTTON1 A0
#define BUTTON2 A1
#define BUTTON3 A2
#define BUTTON4 A3
#define BUTTON5 A4
#define BUTTON6 A5
#define BUTTONS 6

int button1 = 0;
int button2 = 0;
int button3 = 0;
int button4 = 0;
int button5 = 0;
int button6 = 0;

int button1_changed = 0;
int button2_changed = 0;
int button3_changed = 0;
int button4_changed = 0;
int button5_changed = 0;
int button6_changed = 0;

void buttons_init()
{
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  pinMode(BUTTON5, INPUT);
  pinMode(BUTTON6, INPUT);
  digitalWrite(BUTTON1, HIGH); 
  digitalWrite(BUTTON2, HIGH);
  digitalWrite(BUTTON3, HIGH);
  digitalWrite(BUTTON4, HIGH);
  digitalWrite(BUTTON5, HIGH);
  digitalWrite(BUTTON6, HIGH);    
}

// változás jelzők ürítése, minden ciklusban megtörténik
void buttons_changed_clear()
{
  button1_changed = button2_changed = button3_changed = button4_changed = button5_changed = button6_changed = 0;
}

// frissíti a gombok állapotát
void buttons_scan()
{
  static uint32_t lastUpdate = 0;
  
  buttons_changed_clear();
  
  if ((millis() - lastUpdate) < 10)
    return;

  lastUpdate = millis();

  int value;

  value = analogRead(BUTTON1) < 1000;
  button1_changed = button1 != value;
  button1 = value;

  value = analogRead(BUTTON2) < 1000;
  button2_changed = button2 != value;
  button2 = value;
  
  value = analogRead(BUTTON3) < 1000;
  button3_changed = button3 != value;
  button3 = value;

  value = analogRead(BUTTON4) < 1000;
  button4_changed = button4 != value;
  button4 = value;

  value = analogRead(BUTTON5) < 1000;
  button5_changed = button5 != value;
  button5 = value;

  value = analogRead(BUTTON6) < 1000;
  button6_changed = button6 != value;
  button6 = value;  
}

// igazat ad vissza, ha lenyomtak egy gombot, de a felengedését nem figyeli
int buttons_pressed()
{
  int result = 0x00;
  
  if (button1_changed && button1)
    result = 0x01;
  if (button2_changed && button2)
    result = 0x01;
  if (button3_changed && button3)
    result = 0x01;
  if (button4_changed && button4)
    result = 0x01;
  if (button5_changed && button5)
    result = 0x01;
  if (button6_changed && button6)
    result = 0x01;

  return result;
}

// bármilyen változás esetén igazat ad vissza
int buttons_changed()
{
  int result = button1_changed | button2_changed | button3_changed | button4_changed | button5_changed | button6_changed;
  
  return result;
}

// bitfield-be menti a gombok állapotát, ebben a formátumban küldjük el a számítógépnek
uint8_t buttons_pack()
{
  return (button1 << 7) | (button2 << 6) | (button3 << 5) | (button4 << 4) | (button5 << 3) | (button6 << 2);
}
#endif
