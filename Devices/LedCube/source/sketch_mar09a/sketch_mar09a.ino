#include "buttons.h"
#include "particles.h"
#include "display.h"
#include "draw.h"
#include "effect_test.h"
#include "effect_rain.h"
#include "effect_random.h"
#include "effect_snake.h"
#include "effect_scanner.h"
#include "effect_rotate.h"
#include "effect_rotate2.h"
#include "effect_rotate3.h"

/*
 Referencia:
   http://arduino.cc/en/Reference/HomePage#.UyU-ivldV9U
   (Igazából egy C nyelv van mindezek mögött, szóval használhatod az ott megtanult fortélyokat)
 Letöltés:
   http://arduino.cc/en/Main/Software#toc3
   Szükséged lesz a fejlesztő környezetre, 1.5.* vagy annál újabbat tölts le
 Limitációkról itt olvashatsz:
   http://arduino.cc/en/Main/ArduinoBoardNano
   Ez a projekt egy Arduino Nano 3.0-ás változatra készült.
 Sebesség: 
   16 Mhz, 
   32 Kb program memória (-2 Kb bootloader) <- ide kerül kiírása a programod, vagyis 30 Kb-nál nem lehet nagyobb
   2 Kb memória <- ez a dinamikus memória, futássorán felhasználhatü
 Első használat:
   1. Töltsd le a fejlesztő környezetet, ebben található a driver is
   2. Csatlakoztasd USB-n az Arduino Nano-t
   3. Majd a kitömörített fejlesztő környezetre hivatkozva használd a driver-t (mert windows alapból nem ismeri)
     Például nálam ezt a mappát kell megadni: d:\Tools\arduino-1.5.4\drivers 
   4. Általam adott projektet nyísd meg a fejlesztői környezettel
   5. Válazd ki a Tools / Boards / Arduino Nano menüpontot
   6. Válazd ki a Tools / Boards / Ports alatt azt ahova csatlakoztatva van (ezt az eszköz kezelőben is meg tudod nézni, de remélhetőleg csak egy ilyened lesz)
   7. Fájl / Upload segítségével tudod a programodat lefordítani és feltölteni az eszközre (Vagy felül a jobbra nyíl)
   8. Tools / Serial Monitor segítségével üzeneteket tudsz fogadni a kis eszköztől
 */

int demo_mode = 1;
char buffer[8];
int offset = 0;
int automatic_step = 1;
uint32_t automatic_step_timeout = 0;
void setup() 
{
  // gombok inicializálása
  buttons_init();

  // képernyő inicializálása
  display_init();

  // életjel küldése a sorosporton, debug, 
  // ezt láthatod, ha USB-n csatlakoztatva van a gépen és a Fejlesztő környezetben elindítod a Tools/Serial Monitor funkcióját
  // Serial.println <- ezzel bármit bárhol kiküldhetsz a számítógép felé, de arra figyelj, hogy lassítja a rajzolást, de "debug"-hoz tökéletes
  Serial.begin(115200); 
  Serial.println("Szia Kati!"); 

  // várunk, hogy kapjunk a számítógéptől utasítást, ha nem jön, akkor demozunk
  delay(1000);

  // megvizsgáljuk érkezett-e adat, egy számot várunk, amivel kikapcsolhatjuk a demo-t és számítógép utasításokat fog várni
  if (Serial.available())
  {
    demo_mode = Serial.read();
  }

  // demo módban futtunk
  if (demo_mode) 
  {
    // effect beállítások
    effect_rain_setup();
  }
}

void demo()
{
  if (buttons_pressed())
  {
    automatic_step = 0;
    
    if (button6)
    {
      display_toggle_invert_mode();
    } 
    else 
    {
      display_clear();
      demo_mode = (demo_mode + 1) % 12;
      if (demo_mode < 2) demo_mode = 2;
    }
  } 
  else if (automatic_step)
  {
    if (millis() - automatic_step_timeout > 10000)
    {
      automatic_step_timeout = millis();
      
      display_clear();
      demo_mode = (demo_mode + 1) % 12;
      if (demo_mode < 2) demo_mode = 2;    
    }
  }

  if (demo_mode == 2)
  {
    effect_test_step();
  } 
  else if (demo_mode == 3)
  {
    effect_random_step();
  } 
  else if (demo_mode == 4)
  {
    effect_scanner_step();
  } 
  else if (demo_mode == 5)
  {
    effect_rotate_step();
  } 
  else if (demo_mode == 6)
  {
    effect_snake_step();
  } 
  else if (demo_mode == 7)
  {
    effect_rain_step();
  } 
  else if (demo_mode == 8)
  {
    effect_rotate2_step();
  } 
  else if (demo_mode == 9)
  {
    effect_rotate3_step();
  }
  else if (demo_mode == 10)
  {
    display_halffill();
  }
  else if (demo_mode == 11)
  {
    display_fill();
  }
}

void computer()
{
  // számítógéptől érkező parancs
  static int command = 0x00;
  
  if (command == 0x00)
  {
    // nincs parancs feldolgozás
    
    if (buttons_changed())
    {
      Serial.write(0x02);
      Serial.write(buttons_pack());
    }

    if (Serial.available())
    {
      command = Serial.read();
    } 
    else 
    {
      return;
    }
  }
  
  if (command == 0x01)
  {
   // Eszköz paraméterek     
    Serial.write(0x01);
    Serial.write(WIDTH);
    Serial.write(HEIGHT);
    Serial.write(LEVEL);
    Serial.write(BUTTONS);
    
    command = 0x00;
  } 
  else if (command == 0x02)
  {
    // Gombok állapotának küldése
    Serial.write(0x02);
    Serial.write(buttons_pack());

    command = 0x00;    
  }
  else if (command == 0x03)
  {
    display_pack(buffer, 8);
    Serial.write(0x03);
    for(int i=0;i<8;i++)
      Serial.write(buffer[i]);
      
    command = 0x00;      
  }
  else if (command == 0x04)
  {
    // Képernyő fogadás
    if (Serial.available())
    {
      *(buffer + offset++) = Serial.read();
    }
    
    if (offset == 8)
    {
      command = 0x00;
      offset = 0;
      display_extract(buffer, 8);
    }
  }
  else if (command == 0x05)
  {
    // PING
    Serial.write(0x05);
    
    command = 0x00;    
  }
  else
  {
    command = 0x00;
  }  
}

void loop() 
{
  // ezzel rajzolhatod ki a kocka következő szintjét, ezt meg kell hívni
  // ha egy ciklus lassan ér körbe, akkor vibrálni fog a megjelenítés is,
  // mindig gyors kódot kell írni vagy több lépésre bontani a számolást  
  display_scan();

  // gombok állapotának frissítése
  buttons_scan();

  if (demo_mode)
  {
    demo();
  }
  else 
  {
    computer();
  }
}





