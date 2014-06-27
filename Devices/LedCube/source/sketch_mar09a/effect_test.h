#define TEST_STATES 5

static uint32_t lastUpdate = 0;
static int test_mode = 0;
static int counter = 0;
static int state = 0;

void effect_test_step_over_pixels();
void effect_test_step_over_levels();
void effect_test_step_blink();
void effect_test_step_over_lines();

void effect_test_nextstate()
{
  test_mode = (test_mode + 1) % TEST_STATES;
  lastUpdate = 0;
  state = 0;
  counter = 0;
}

void effect_test_step()
{
  if (test_mode == 0)
    effect_test_step_over_pixels();
  else if (test_mode == 1)
    effect_test_step_over_lines();
  else if (test_mode == 2)    
    effect_test_step_over_levels();
  else if (test_mode == 3)
    effect_test_step_blink();
  else if (test_mode == 4)
  {
    if ((millis() - lastUpdate) > 5000)
    {
      lastUpdate = millis();
      effect_test_nextstate();
    } 
    else 
    {
      display_fill();
    }
  }
}

void effect_test_step_over_pixels()
{
  if ((millis() - lastUpdate) > 50)
  {
    lastUpdate = millis();      
    display_clear();
    display_buffer[state] = 0x01;
    state = (state + 1) % TOTAL;

    if (state == 0)
    {
      effect_test_nextstate();
    }
  }
}

void effect_test_step_over_lines()
{
  if ((millis() - lastUpdate) > 100)
  {
    lastUpdate = millis();      
    display_clear();

    uint8_t *level = (display_buffer + (state * WIDTH));
    state = (state + 1) % MATRIX;

    for( int i=0; i < WIDTH; i++)
    {
      level[i] = 0x01;
    }
    
    if (state == 0)
      ++counter;

    if (counter > 0)
    {
      counter = 0;
      effect_test_nextstate();
    }
  }
}

void effect_test_step_over_levels()
{
  if ((millis() - lastUpdate) > 200)
  {
    lastUpdate = millis();      
    display_clear();

    uint8_t *level = (display_buffer + (state * MATRIX));
    state = (state + 1) % LEVEL;

    for( int i=0; i < MATRIX; i++)
    {
      level[i] = 0x01;
    }
    
    if (state == 0)
      ++counter;

    if (counter > 0)
    {
      counter = 0;
      effect_test_nextstate();
    }
  }
}

void effect_test_step_blink()
{
  if ((millis() - lastUpdate) > 100)
  {
    lastUpdate = millis();
    if (state)
      display_clear();
    else
      display_fill();

    state = (state + 1) % 2;

    ++counter;

    if (counter > 20)
    {
      counter = 0;
      effect_test_nextstate();
    }
  }
}

