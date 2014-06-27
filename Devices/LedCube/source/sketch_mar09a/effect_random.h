void effect_random_step()
{
  static int counter = 0;
  static int state = 0x01;
  static uint32_t lastUpdate = 0;
  if ((millis() - lastUpdate) > 100)
  {
    lastUpdate = millis();
    
    display_buffer[random() % TOTAL] = state;
    ++counter;
    
    if (counter >= TOTAL)
    {
      counter = 0;
      randomSeed(micros()); 
      //display_clear();
      state = (state + 1) % 2;
    }
  }
}
