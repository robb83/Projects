#ifndef PARTICLES_H
#define PARTICLES_H
#include <stdio.h>
#include <stdlib.h> 
#include "display.h"

/* Részecske adatai */
typedef struct SParticle 
{
  float x;
  float y;
  float z;
  float speed;
} Particle;

/* Részecske lista */
typedef struct SParticles
{
  int count;
  Particle *particles;
} Particles;

/* Random pozicióba létrehoz a paraméterben megadott számú részecskét */
Particles* generateParticles(int count)
{
  Particles *particles = (Particles*)malloc(sizeof(Particles));
  particles->count = count;
  particles->particles = (Particle*)malloc(sizeof(Particle) * count);
  
  for(int i=0; i < count; i++)
  {
    particles->particles[i].x = (float)(random() % 4);
    particles->particles[i].y = (float)(random() % 4);
    particles->particles[i].z = (float)(random() % 4);
    particles->particles[i].speed = 1 / ((float)(random() % 4));
  }
  
  return particles;
}

/* Létrehozott részecskék felszabadítása, erre akkor van szükség, ha már nem szeretnéd használni */
void freeParticles(Particles* particles)
{
  if (!particles)
    return;
    
  if (particles->particles)
  {
    free(particles->particles);
    particles->particles = NULL;
  }
  
  free(particles);
}

/* Részecske animálása a sebességétől függően */
void applyGravity(Particles* particles)
{
  if (!particles)
    return;
    
  for(int i=0;i<particles->count;i++)
  {
    Particle *p = &particles->particles[i];
    p->z = p->z - p->speed;
    
    if (p->z < 0)
    {
      p->x = (float)(random() % 4);
      p->y = (float)(random() % 4);
      p->z = 3;
      p->speed = 1 / ((float)(random() % 4));
    }
  }
}

/* Részecske kupac megjelenítése */
void particleDisplay(Particles *particles)
{
  display_clear();

  for( int i=0; i < particles->count; i++)
  {
    int z = particles->particles[i].z;
    int x = particles->particles[i].x;
    int y = particles->particles[i].y;

    if (z < 0 || z >= LEVEL)
      continue;

    *(display_buffer + (z * MATRIX) + (x + y * WIDTH)) = 0x01;
  }
}
#endif
