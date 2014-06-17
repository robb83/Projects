import ddf.minim.*;
import ddf.minim.ugens.*;
import ddf.minim.signals.*;
import javax.sound.sampled.*;

AudioSample bulletSoundWave;

void playHitSound() {  
  bulletSoundWave.trigger();
}

void generateSound() {
  Minim minim = new Minim(this);
  int size = 512;
  float[] samples = new float[size * 8];
  SineWave sine = new SineWave( 1200, 0.3, 44100);
  sine.generate(samples);  
  AudioFormat format = new AudioFormat( 44100, 16, 1, true, true );
  bulletSoundWave = minim.createSample( samples, format, size);  
}
