Target createRandomTarget() {
  Target t = new Target();
  t.visible = true;
  t.hitted = false;
  t.targetR = random(50, 100);
  t.position[0] = random(0 + t.targetR, width - t.targetR);
  t.position[1] = random(0 + t.targetR, height - t.targetR);
  return t;
}
  
class Target {
  public boolean hitted;
  public boolean visible;
  public float[] position = new float[2];
  public float targetR = 10;
  public float distance = 0.0f;
  public float speed = 1f;
  public float maxDistance = 20.0f;
  public int fillColor = color(255, 0, 0);
  
  public boolean hit(Player player) {
      float dx = player.screenPosition[0] - position[0];
      float dy = player.screenPosition[1] - position[1];
    
      float d = abs(sqrt(dx * dx + dy * dy));
   
      return d <= targetR / 2;    
  }
  
  public void draw() {     
      if (!visible)
        return;
        
      noStroke();
        
      if (!hitted) {
        fill(fillColor);
        ellipse(position[0], position[1], targetR, targetR);
        
        return;
      }
      
      fill(fillColor, 255 * ((maxDistance - distance) / 20));
      for(float a = npi; a <= 2 * PI; a += npi) {
        
        float ha = a - (hnpi);
        
        float x = position[0] + (distance * cos(ha));
        float y = position[1] + (distance * sin(ha));
        
        arc(x, y, targetR, targetR, a - npi, a);
      }
      
      if (distance < maxDistance) distance += speed;
      else visible = false;
  }
}
