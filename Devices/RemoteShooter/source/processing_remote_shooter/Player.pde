class Player {
  public String name;
  public boolean connected;
  public String ip;
  public int port;
  public int points;
  public int lastMessageReceived;
  public int lastMessageSended;
  
  public int pointerColor = color(255, 255, 255);  
  public float[] position = new float[3];
  public float[] screenPosition = new float[2];
  
  public boolean requestReset;
  
  public void draw(int i) {

    String statusText = (connected ? (requestReset ? " (R)" : "") : " (D)");
    String playerText = name + ": " + points + statusText;

    fill(pointerColor);
    text(playerText, 20, 150 + (i * 50));
    
    noFill();
    stroke(pointerColor);
    if (connected)
      point(screenPosition[0], screenPosition[1]);        
  }
}
