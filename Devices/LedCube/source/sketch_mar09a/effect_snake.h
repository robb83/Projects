int effect_snake_possible(int x, int y, int z);

void effect_snake_step()
{
  static uint32_t lastUpdate = 0;
  static int snake_dead = 0;
  static int snake_head = 0;
  static int directions[] = {0, 0, 0, 0, 0, 0};
  
  // várunk kicsit
  if ((millis() - lastUpdate) <= 200)
    return;    
  lastUpdate = millis();
  
  // kirajzoljuk a lépést
  if (snake_dead)
  {
    randomSeed(micros());
    display_clear();
    snake_dead = 0;
  }
  
  display_buffer[snake_head] = 0x01;
  
  // koordináták meghatározása
  int z = snake_head / MATRIX;  
  int x = (snake_head - (z * MATRIX)) % WIDTH;
  int y = (snake_head - (z * MATRIX)) / HEIGHT;
  
  // kiszámoljuk a következő lépést
  int possibles = 0;
  int step = -1;
  
  step = effect_snake_possible(x + 1, y, z);
  if (step > -1)
    directions[possibles++] = step;
  
  step = effect_snake_possible(x - 1, y, z);
  if (step > -1)
    directions[possibles++] = step;
  
  step = effect_snake_possible(x, y + 1, z);
  if (step > -1)
    directions[possibles++] = step;
  
  step = effect_snake_possible(x, y - 1, z);
  if (step > -1)
    directions[possibles++] = step;
  
  step = effect_snake_possible(x, y, z + 1);
  if (step > -1)
    directions[possibles++] = step;
  
  step = effect_snake_possible(x, y, z - 1);
  if (step > -1)
    directions[possibles++] = step;
    
  if (possibles == 0)
  {
    snake_head = random() % TOTAL;
    snake_dead = 1;
  } 
  else 
  {
    snake_head = directions[random() % possibles]; 
  }
}

int effect_snake_possible(int x, int y, int z)
{
  if (x < 0 || x >= WIDTH)
    return -1;
  
  if (y < 0 || y >= HEIGHT)
    return -1;
  
  if (z < 0 || z >= LEVEL)
    return -1;
    
  int index = (y * WIDTH + x) + (z * MATRIX);
  
  if (index < 0)
    return -1;
    
  if (index > TOTAL)
    return -1;
  
  if (display_buffer[index])
    return -1;
  
  return index;
}

