void effect_rotate3_step_layer(int level, int counter);

void effect_rotate3_step()
{

  static uint32_t lastUpdate = 0;
  static int counter = 0;
  static int s = 0;
  static int pixelorder[] = {
    0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4   };

  // várunk kicsit
  if ((millis() - lastUpdate) <= 100)
    return;
  lastUpdate = millis();

  display_clear();

  int level = 0;

  if (s > 120)
  {
    s = 0;
  }

  if (s > 60)
  {
    display_buffer[level + pixelorder[counter]] = 0x01;
    display_buffer[level + pixelorder[(counter + 1) % 12]] = 0x01;
    display_buffer[level + pixelorder[(counter + 2) % 12]] = 0x01;
    
    level += MATRIX;
    
    display_buffer[level + pixelorder[(counter + 1) % 12]] = 0x01;
    display_buffer[level + pixelorder[(counter + 2) % 12]] = 0x01;
    display_buffer[level + pixelorder[(counter + 3) % 12]] = 0x01;
    
    level += MATRIX;
    
    display_buffer[level + pixelorder[(counter + 2) % 12]] = 0x01;
    display_buffer[level + pixelorder[(counter + 3) % 12]] = 0x01;
    display_buffer[level + pixelorder[(counter + 4) % 12]] = 0x01;
    
    level += MATRIX;
    
    display_buffer[level + pixelorder[(counter + 3) % 12]] = 0x01;
    display_buffer[level + pixelorder[(counter + 4) % 12]] = 0x01;
    display_buffer[level + pixelorder[(counter + 5) % 12]] = 0x01;
  } 
  else 
  {
    display_buffer[level + pixelorder[counter]] = 0x01;
    display_buffer[level + pixelorder[(counter + 1) % 12]] = 0x01;
    display_buffer[level + pixelorder[12 + (counter - 1)]] = 0x01;

    level += MATRIX;

    display_buffer[level + pixelorder[11 - counter]] = 0x01;
    display_buffer[level + pixelorder[11 - ((counter + 1) % 12)]] = 0x01;
    display_buffer[level + pixelorder[11 - ((12 + (counter - 1)) % 12)]] = 0x01;

    level += MATRIX;

    display_buffer[level + pixelorder[counter]] = 0x01;
    display_buffer[level + pixelorder[(counter + 1) % 12]] = 0x01;
    display_buffer[level + pixelorder[12 + (counter - 1)]] = 0x01;

    level += MATRIX;

    display_buffer[level + pixelorder[11 - counter]] = 0x01;
    display_buffer[level + pixelorder[11 - ((counter + 1) % 12)]] = 0x01;
    display_buffer[level + pixelorder[11 - ((12 + (counter - 1)) % 12)]] = 0x01;
  }     

  counter = (counter + 1) % 12;
  ++s;
}

