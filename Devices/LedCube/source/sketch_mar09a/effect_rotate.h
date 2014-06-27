void effect_rotate_step()
{
  static uint32_t lastUpdate = 0;
  static int s = 0;
  static int counter = 0;
  static int shift = 0;
  
  // várunk kicsit
  if ((millis() - lastUpdate) <= 150)
    return;
  lastUpdate = millis();

  display_clear();

  if (s == 0)
    draw_front(0);
  else if (s == 1)
    draw_side(WIDTH - 1);
  else if (s == 2)
    draw_front(HEIGHT - 1);
  else if (s == 3)
    draw_side(0);
  
  else if (s == 4)
    draw_level(0);
  else if (s == 5)
    draw_side(WIDTH - 1);
  else if (s == 6)
    draw_level(LEVEL - 1);
  else if (s == 7)
    draw_side(0);
    
  else if (s == 8)
    draw_level(LEVEL - 1);
  else if (s == 9)
    draw_front(HEIGHT - 1);
  else if (s == 10)
    draw_level(0);
  else if (s == 11)
    draw_front(0);
    
  ++counter;
  if (counter > 20)
  {
    counter = 0;
    shift += 4;
    if (shift > 8) shift = 0;
  }
  
  s = shift + ((s + 1) % 4);
}

