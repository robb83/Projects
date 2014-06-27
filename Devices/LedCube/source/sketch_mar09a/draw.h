// Általános, újrafelhasználható rajzoló és egyéb számoló függvények
// Sporolni kell, mert elég kevés a tároló kapacítás és a memória

// kocka elejét kitölti az egész lapot, 0-tól (HEIGHT - 1)-ig vár paramétert
void draw_front(int line)
{
  uint8_t *ptr = (display_buffer + (line * WIDTH));
  for( int i=0; i < WIDTH; i++)
  {
    for( int z=0; z < LEVEL * MATRIX; z+= MATRIX)
    {
      *(ptr + z) = 0x01;
    }
    ++ptr;
  }
}

// kocka oldalát tölti fel, 0-tól (WIDTH - 1)-ig vár paramétert
void draw_side(int line)
{
  for( int i=0; i < HEIGHT; i++)
  {
    uint8_t *start = (display_buffer + (i * WIDTH)) + line;

    for( int z=0; z < LEVEL; z++)
    {
      *(start + (z * MATRIX)) = 0x01;
    }
  }
}

// kocka lapjait tölti fel, 0-tól (LEVEL - 1)-ig vár paramétert
void draw_level(int line)
{
  uint8_t *level = (display_buffer + (line * MATRIX));

  for(int i=0; i < MATRIX; i++)
  {
    level[i] = 0x01;
  }
}

// kocka egy pixelét állítja
void draw_pixel(int x, int y, int z, int value)
{
  display_buffer[(z * MATRIX) + (y * WIDTH) + (x)] = value;
}
