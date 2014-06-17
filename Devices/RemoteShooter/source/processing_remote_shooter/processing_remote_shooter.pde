import ddf.minim.*;
import hypermedia.net.*;

UDP udp;
Player[] players = new Player[4];
int[] playerColors = new int[] { 
  color(255, 255, 255), color(255, 255, 0), color(0, 255, 0), color(0, 255, 255), 
};
ArrayList<Target> targets = new ArrayList<Target>();
int lastReset = 0;
int elapsedGameTime = 0;

boolean fullscreen = false  ;
boolean sketchFullScreen() {
  return fullscreen;
}

void setup() {
  if (fullscreen) {
    size(displayWidth, displayHeight);
  } else {
    size(1024, 768);
  }
  
  noCursor();
  ellipseMode(CENTER);
  udp = new UDP( this, 5545 );
  udp.listen(true); 
  
  generateSound();
  reset();
}

void drawTargets() {
  for(int i=targets.size() - 1; i >= 0; i--) {
    Target t = targets.get(i);
    
    if (t.visible)
      t.draw();
    else
      targets.remove(i);
  }
}

void drawTimer() {
  int hour = (elapsedGameTime / 3600000);
  int minutes = (elapsedGameTime / 60000 - (hour * 60));
  int seconds = (elapsedGameTime / 1000 - (hour * 60 * 60) - (minutes * 60));
  
  String time = "";
  if (hour < 10) time += "0" + hour;
  else time += ":" + hour;
  if (minutes < 10) time += ":0" + minutes;
  else time += ":" + minutes;
  if (seconds < 10) time += ":0" + seconds;
  else time += ":" + seconds;
  
  noStroke();
  textSize(26);
  fill(255, 255, 0);
  text("Time: " + time , (width - 200) / 2, 30);  
}

void drawPlayers() {
  textSize(16);
  strokeWeight(5);
  noFill();
  
  for (int i=0;i<players.length;i++) {
    if (players[i] == null)
      continue;

      players[i].draw(i);
  }
}

void draw() {
  heartbeat();
  background(0);
  
  if (onlinePlayers() > 0) {
      elapsedGameTime = millis() - lastReset;
  }
  
  drawTargets();
  drawTimer();
  drawPlayers();
}

void hit(Player player) {
  for(int i=targets.size() - 1;i>=0;i--) {
      Target t = targets.get(i);
      
      if (t.hitted)
        continue;
      
      if (t.hit(player)) {
        t.hitted = true;
        t.fillColor = player.pointerColor;
        player.points += 1;
        targets.add(createRandomTarget());
        
        playHitSound();
        playerSend(player, "a:a");
        break;
     }
  }
}

int onlinePlayers() {
  int online = 0;
  
  for(int i=0; i < players.length;i++) {
    if (players[i] == null || !players[i].connected)
      continue;
      
      ++online;
  }
  
  return online;
}

void reset() { 
  lastReset = millis();
  elapsedGameTime = 0;
  targets.clear();
  targets.add(createRandomTarget());
    
  for(int i=0; i < players.length;i++) {
    if (players[i] == null)
      continue;
      
    if (!players[i].connected) {
      players[i] = null;
      continue;
    }
      
    players[i].requestReset = false;
    players[i].points = 0;
    players[i].position[0] = 0;
    players[i].position[1] = 0;
    players[i].position[2] = 0;
    players[i].screenPosition[0] = 0;
    players[i].screenPosition[1] = 0;
  }  
}

void requestReset(Player player) {
  player.requestReset = true;
  
  for(int i=0; i < players.length; i++) {
    if (players[i] == null || !players[i].connected)
      continue;
    
    if (!players[i].requestReset) return;
  }
  
  reset();
}

void processDeviceOrientation(Player player, float x, float y, float z) {
   // magnetic + accelerometer mode
   y = cut(y);y += npi;
   z = cut(z);z += npi;
   
   player.position[0] = map(z, 0, npi, 0,  width / 2) - (width / 2);
   player.position[1] = map(y, 0, npi, 0,  height / 2) - (height / 2);
  
   preparePlayerPosition(player);
}

void processDeviceAccelerometer(Player player, float x, float y, float z) {
   // accelerometer mode
   player.position[0] += x;
   player.position[1] += y;
   player.position[2] += z;
   
   preparePlayerPosition(player);
}

void preparePlayerPosition(Player player) {
   player.screenPosition[0] = player.position[0] + (width / 2);
   player.screenPosition[1] = player.position[1] + (height / 2);
   
   if (player.screenPosition[0] > width) 
   {
     player.screenPosition[0] = width;
     player.position[0] = width / 2;
   }
   
   if (player.screenPosition[0] < 0) 
   {
     player.screenPosition[0] = 0;
     player.position[0] = -(width / 2);
   }

   if (player.screenPosition[1] > height) 
   {
     player.screenPosition[1] = height;
     player.position[1] = height / 2;
   }
   
   if (player.screenPosition[1] < 0) 
   {
     player.screenPosition[1] = 0;
     player.position[1] = -(height / 2);
   }  
}

void playerConnected() {
  if (onlinePlayers() == 1) {
    reset();
  }
}

void playerDisconnected() {
  
}
