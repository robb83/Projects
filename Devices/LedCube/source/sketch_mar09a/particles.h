#ifndef PARTICLES_H
#define PARTICLES_H
#include <stdio.h>
#include <stdlib.h> 
#include "display.h"

typedef struct SParticle 
{
  float x;
  float y;
  float z;
  float speed;
} Particle;

typedef struct SParticles
{
  int count;
  Particle *particles;
} Particles;

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

// részecskék megjelenítése
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
