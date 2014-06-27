// gombok kezelését segítő metódusok
#include "buttons.h"
// képernyő megjelenítésért felelős metódusok
#include "display.h"
// pár rajzolást segítő függvény
#include "draw.h"
#include "particles.h"

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
 
void setup() 
{
  // gombok inicializálása
  buttons_init();

  // képernyő inicializálása
  display_init();

  // életjel küldése a sorosporton, debug, 
  // ezt láthatod, ha USB-n csatlakoztatva van a gépen és a Fejlesztő környezetben elindítod a Tools/Serial Monitor funkcióját és alul a legördülő listában válazd ki a 115200-at
  // Serial.println <- ezzel bármit kiküldhetsz a számítógép felé, de arra figyelj, hogy lassítja a rajzolást, "debug"-hoz tökéletes
  // Ezeket bármilyen programmal kitudod olvasni, a kulcsszó a Soros port kezelés (pl: C# SerialPort, de erre is csináltam egy példát)
  Serial.begin(115200);
  Serial.println("Szia Kati!");
}

int index = 0;
uint32_t last_timeout = 0;

void loop() 
{
  // ezzel rajzolhatod ki a kocka következő szintjét, ezt meg kell hívni
  // ha egy ciklus lassan ér körbe, akkor vibrálni fog a megjelenítés is,
  // mindig gyors kódot kell írni vagy több lépésre bontani a számolást  
  display_scan();

  // gombok állapotának frissítése
  buttons_scan();

  // így tudhatod meg, hogy lenyomták-e valamelyik gombot
  if (buttons_pressed())
  {
    Serial.println("Button pressed.");
  }
  
  // így tudhatod meg, hogy változott-e valamelyik gomb állapota
  if (buttons_changed())
  {
    Serial.println("Button state changed.");
  }
  
  // így tudhatod meg, hogy az 1-es gombot lenyomták (jelenleg BUTTONS darab van)
  if (button1_changed && button1)
  {
    Serial.println("Button1");
  }

  // így tudsz várakozni tetszőleges ideig, fontos, hogy a rajzálás közben végrehajtódjön, ezért nem használjuk a delay vagy a delayMicroseconds függvényeket.
  // 1000 = 1 másodperc
  if (millis() - last_timeout > 100)
  {
    last_timeout = millis();
    
    // ide jön maga a rajzólás
    display_buffer[index] = !display_buffer[index]; // display_buffer maga a rajzterület: WIDTH * HEIGHT * LEVEL méretű tömbs, összesen TOTAL elemű egy szinten MATRIX elem lehet
    index = (index + 1) % TOTAL;
  }
}





