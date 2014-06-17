
void process(Player player, String[] values) {
    if (values == null || values.length == 0)
      return;
      
    if ("b".equals(values[0]))
    {
      if (values.length > 3) {
        float a = float(values[1]);
        float b = float(values[2]);
        float c = float(values[3]);
    
        processDeviceAccelerometer(player, a, b, c);
      }
    }
    else if ("c".equals(values[0]))
    {
      if (values.length > 3) {
        float a = float(values[1]);
        float b = float(values[2]);
        float c = float(values[3]);
    
        processDeviceOrientation(player, a, b, c);
      }
    }
    else if ("a".equals(values[0]))
    {
      if (values.length > 1) {
        if (values[1].equals("a"))
        {
          hit(player);
        }
        else if (values[1].equals("b"))
        {   
          
        }  
        else if (values[1].equals("c"))
        {    
          requestReset(player);
        }
      }
    }
}

void playerSend(Player player, String message) {
  if (player == null || !player.connected)
    return;
    
   udp.send(message, player.ip, player.port );
   player.lastMessageSended = millis();
}

void heartbeat() {

  int currentMillisec = millis();
  
  for(int i=0;i<players.length;i++) {
    if (players[i] == null)
      continue;
      
    if (players[i].connected) {
      if (currentMillisec - players[i].lastMessageReceived > 4000) {
        disconnect(players[i]);
      }
    }
   
    if (players[i].connected) {
      if (currentMillisec - players[i].lastMessageSended > 500 ) {
        playerSend(players[i], "heartbeat");
      }
    }  
  }
}

void disconnect(Player player) {
  if (player != null && player.connected) {
    player.connected = false;    
    playerDisconnected();
    println("Disconnected: " + player.ip + ":" + player.port + " (" + player.name + ")");
  }
}

Player connecting(String ip, int port, String[] values) {
  Player player = null;
  
  if (values != null && values.length > 1) {
    String name = values[1];
    if (name == null)
      return null;
    
    name = name.trim();
    
    if (name.length() == 0 || name.length() > 20)
      return null;
    
    player = insertPlayer(ip, port, name);
    if (player != null) {        
      playerSend(player, "connect:accepted:" + player.name);
      playerConnected();
      println("Connection accepted from: " + player.ip + ":" + player.port + " (" + player.name + ")");
    }
  }
 
 return player; 
}

void receive( byte[] data, String ip, int port ) {
  String[] values = split(new String(data), ':');
  
  if (values == null || values.length == 0)
    return;
 
  Player player = findPlayer(ip, port);
  
  if (player != null) {
    player.lastMessageReceived = millis();
  }
  
  if (player == null || !player.connected) {
    if ("connect".equals(values[0]))
    {
      connecting(ip, port, values);
    }
  } 
  else {
    if ("disconnect".equals(values[0])) 
    {
      disconnect(player);
    } else {
      process(player, values);
    }
  }
}

Player findPlayer(String ip, int port) {
  for(int i=0; i< players.length; i++) {
    if (players[i] == null) continue;
    
    if (players[i].ip.equals(ip) && players[i].port == port) {
      return players[i];
    }
  }
  return null;
}

Player findPlayer(String name) {
  for(int i=0; i< players.length; i++) {
    if (players[i] == null) continue;
    if (!players[i].connected) continue;
    
    if (players[i].name.equals(name)) {
      return players[i];
    }
  }
  return null;
}

Player insertPlayer(String ip, int port, String name) {
  
  if (findPlayer(name) != null)
    return null;
    
  for(int i=0; i< players.length; i++) {
    if (players[i] == null || !players[i].connected) {
      players[i] = new Player();
      players[i].connected = true;
      players[i].name = name;
      players[i].ip = ip;
      players[i].port = port;
      players[i].lastMessageReceived = millis();
      players[i].pointerColor = playerColors[i];
      
      return players[i];
    }
  }
  
  return null;
}
