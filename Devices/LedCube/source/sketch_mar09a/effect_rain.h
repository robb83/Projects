static Particles *particles = NULL;

void effect_rain_setup()
{
  freeParticles(particles);
  particles = generateParticles(10);  
}

void effect_rain_step()
{
  static uint32_t lastUpdate = 0;
  if ((millis() - lastUpdate) > 100)
  {
    lastUpdate = millis();

    particleDisplay(particles);    
    applyGravity(particles);
  }  
}

