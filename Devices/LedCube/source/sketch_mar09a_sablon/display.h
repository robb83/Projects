#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

// shift regiszter vezérlő portok (PORTD)
#define SER 2
#define OE 3
#define RCLK 4
#define SRCLK 5
#define SRCLR 6

#define LEVEL1 12
#define LEVEL2 11
#define LEVEL3 10
#define LEVEL4 9

// képernyő adatai
#define WIDTH (4)
#define HEIGHT (4)
#define LEVEL (4)
#define MATRIX (WIDTH * HEIGHT)
#define TOTAL (MATRIX * LEVEL)

// Port műveletek gyorsítása közvetlen hozzáféréssel
#define SER_HIGH (PORTD = (PORTD & (~(0x01 << SER))) | (0x01 << SER))
#define SER_LOW (PORTD = (PORTD & (~(0x01 << SER))))

#define SRCLR_HIGH (PORTD = (PORTD & (~(0x01 << SRCLR))) | (0x01 << SRCLR))
#define SRCLR_LOW (PORTD = (PORTD & (~(0x01 << SRCLR))))

#define SRCLK_HIGH (PORTD = (PORTD & (~(0x01 << SRCLK))) | (0x01 << SRCLK))
#define SRCLK_LOW (PORTD = (PORTD & (~(0x01 << SRCLK))))

#define RCLK_HIGH (PORTD = (PORTD & (~(0x01 << RCLK))) | (0x01 << RCLK))
#define RCLK_LOW (PORTD = (PORTD & (~(0x01 << RCLK))))

#define OE_HIGH (PORTD = (PORTD & (~(0x01 << OE))) | (0x01 << OE))
#define OE_LOW (PORTD = (PORTD & (~(0x01 << OE))))

// word = byte, vagyis a legkisebb címezhető egység, 
// ami gyorsabb futást eredményez, mint a bitenkénti művelet (TODO: talán konkrét avr asm-el lehetne gyorsabbat írni), 
// de több memóriát eszik, nekem most nem kellett sporolnom
// 64 byte vs 8 byte
uint8_t display_buffer[TOTAL] = 
{
  // level 1
  0x01, 0x01, 0x01, 0x01, 
  0x01, 0x00, 0x00, 0x01, 
  0x01, 0x00, 0x00, 0x01, 
  0x01, 0x01, 0x01, 0x01,
  
  // level 2
  0x01, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0x01,
  
  // level 3
  0x01, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0x01,
  
  // level 4
  0x01, 0x01, 0x01, 0x01, 
  0x01, 0x00, 0x00, 0x01, 
  0x01, 0x00, 0x00, 0x01, 
  0x01, 0x01, 0x01, 0x01,
};

static int invert_mode = 0;

void display_init()
{
  // vezérléshez a portokat konfiguráljuk és beállítjuk az alapértelmezett értékeit
  pinMode(SER, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SRCLR, OUTPUT);
  pinMode(LEVEL1, OUTPUT);
  pinMode(LEVEL2, OUTPUT);
  pinMode(LEVEL3, OUTPUT);
  pinMode(LEVEL4, OUTPUT);

  // kijelző törlése, bekapcsolás
  digitalWrite(OE, HIGH);
  digitalWrite(SRCLR, LOW);
  digitalWrite(SRCLR, HIGH);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  digitalWrite(LEVEL1, HIGH);
  digitalWrite(LEVEL2, HIGH);
  digitalWrite(LEVEL3, HIGH);
  digitalWrite(LEVEL4, HIGH);
  //digitalWrite(OE, LOW);
}

// kirajzolja a kocka következő szintjét és lép a következő szintre, ezt mindig hívni kell
void display_scan()
{
  static uint8_t scanline = 0;
  
  // töröljük a regiszter tartalmát
  // erre elvileg nincs szükség, mert felülírjuk
  //SRCLR_LOW;
  //SRCLR_HIGH;
  
  // meghatározzuk az aktuális szint memória címét
  uint8_t *line = (display_buffer + (scanline * MATRIX));

  // címről felolvasunk 16 byte (MATRIX) adatot és elküldjük a regiszternek
  // fordítva olvassuk ki, mert tükrözni kell a megjelenítés miatt: előbb kell beküldeni az utolsó pixeleket
  for(int a = 0; a < MATRIX; a++)
  {
    SRCLK_LOW;
    if ((invert_mode && !line[a]) || (!invert_mode && line[a]))
    {
      SER_HIGH;
    } 
    else 
    {
      SER_LOW;      
    }
    SRCLK_HIGH; // beolvasstatjuk a most küldött jelet és léptetjük a regisztert
  }
  
  // a beküldött adatok tárolása, ennek hatására vált át a LED megjelenítés
  RCLK_LOW;
  RCLK_HIGH;

  digitalWrite(LEVEL1, (scanline == 0));
  digitalWrite(LEVEL2, (scanline == 1));
  digitalWrite(LEVEL3, (scanline == 2));
  digitalWrite(LEVEL4, (scanline == 3));
  
  OE_LOW;
  delayMicroseconds(125);
  OE_HIGH;
  
  digitalWrite(LEVEL1, LOW);
  digitalWrite(LEVEL2, LOW);
  digitalWrite(LEVEL3, LOW);
  digitalWrite(LEVEL4, LOW);
  
  // léptetjük a következő szintre
  scanline = (scanline + 1) % LEVEL;
}

void display_clear()
{
    memset(display_buffer, 0x00, TOTAL);
}

void display_fill()
{
    memset(display_buffer, 0x01, TOTAL);
}

void display_invert()
{
  for(int i=0;i<TOTAL;i++)
    display_buffer[i] = !display_buffer[i];
}

void display_halffill()
{
  for(int i=0, s = 0; i < TOTAL;i++)
  {
    display_buffer[i] = s;
    s = !s;
    
    if ((i % 4) == 3) 
      s = !s;
    if ((i % 16) == 15)
      s = !s;
  }
}

void display_fill3()
{
  memset(display_buffer, 0x00, TOTAL);
  for(int i=0;i < TOTAL;i+=3)
    display_buffer[i] = 0x01;
}

void display_toggle_invert_mode()
{
  invert_mode = !invert_mode;
}

void display_pack(char *buffer, uint8_t length)
{
  if (length < TOTAL / 8)
    return;
    
  for(int i = 0; i < TOTAL / 8; i++)
  {
    char c = 0;
    
    for(int b = 0; b < 8; b++)
    {
       c |= (display_buffer[(i * 8) + b]) << b;
    }
    
    buffer[i] = c;
  }
}

void display_extract(char *buffer, uint8_t length)
{
  for(int i=0; i < length;i++)
  {
    char c = buffer[i];
    
    for(int b = 0; b < 8; b++)
    {
      display_buffer[(i * 8) + b] = ((c & (1 << (7 - b))) >> (7 - b));
    }
  }
}
/*
void display_rotate1(int level, int step)
{
  static int rotate[] = { 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4 };
  
  while (step > 11)
    step -= 11;
    
  uint8_t *level = (display_buffer + (level * MATRIX));

  for(int s = 0; s < step; s++)
  {
    
  }
}*/
#endif
