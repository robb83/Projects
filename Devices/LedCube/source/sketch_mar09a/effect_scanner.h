#define SCANNER_STATES 6
static int effect_scanner_state = 0;
static int effect_scanner_line = 0;

void effect_scanner_left();
void effect_scanner_right();
void effect_scanner_up();
void effect_scanner_down();
void effect_scanner_back();
void effect_scanner_fore();

void effect_scanner_step()
{
  static uint32_t lastUpdate = 0;
  
  // várunk kicsit
  if ((millis() - lastUpdate) <= 100)
    return;    
  lastUpdate = millis();
  
  display_clear();
  
  if (effect_scanner_state == 0)
    effect_scanner_left();
  else if (effect_scanner_state == 1)
    effect_scanner_right();
  else if (effect_scanner_state == 2)
    effect_scanner_up();
  else if (effect_scanner_state == 3)
    effect_scanner_down();
  else if (effect_scanner_state == 4)
    effect_scanner_back();
  else if (effect_scanner_state == 5)
    effect_scanner_fore();
}

void effect_scanner_nextstate()
{
  effect_scanner_state = (effect_scanner_state + 1) % SCANNER_STATES;
}

void effect_scanner_left()
{
  static int line = HEIGHT - 1;
  
  display_clear();
  
  line = (line + 1) % HEIGHT;
  draw_side(line);
  
  if (line == HEIGHT - 1)
    effect_scanner_nextstate();
}

void effect_scanner_right()
{
  static int line = 0;
  
  display_clear();
  
  line = (HEIGHT + (line - 1)) % HEIGHT;
  draw_side(line);
  
  if (line == 0)
    effect_scanner_nextstate();  
}

void effect_scanner_up()
{
  static int line = LEVEL - 1;
  
  display_clear();
  
  line = (line + 1) % LEVEL;
  draw_level(line);
  
  if (line == LEVEL - 1)
    effect_scanner_nextstate();    
}

void effect_scanner_down()
{
  static int line = 0;
  
  line = (LEVEL + (line - 1)) % LEVEL;
  draw_level(line);
  
  if (line == 0)
    effect_scanner_nextstate();    
}

void effect_scanner_back()
{
  static int line = 0;
  line = (line + 1) % WIDTH;
     
  draw_front(line);
  
  if (line == WIDTH - 1)
    effect_scanner_nextstate();
}

void effect_scanner_fore()
{  
  static int line = 0;
  line = (WIDTH + (line - 1)) % WIDTH;
  
  draw_front(line);
  
  if (line == 0)
    effect_scanner_nextstate();          
}

