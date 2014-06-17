package com.simple.remoteshooter;

import java.net.InetSocketAddress;

/**
 * Created by robb83@gmail.com on 2014.05.23..
 */
public class RemoteControllerPlayer {
    final int STATE_PENDING_CONNECT = 0;
    final int STATE_CONNECTED = 1;
    final int STATE_PENDING_DISCONNECT = 2;
    final int STATE_DISCONNECTED = 3;

    final long T_HEARTBEAT = 1000;

    InetSocketAddress remoteAddress;
    String playerName;
    boolean magneticSensor;
    RemoteControllerClientChannel channel;
    RemoteControllerPlayerListener listener;
    Thread channelThread;
    Thread heartBeatThread;
    volatile int state;
    long lastMessageSended;
    long lastMessageReceived;
    long connectingStarted;
    Object syncLock = new Object();

    private RemoteControllerClientListener mClientListener = new RemoteControllerClientListener() {
        @Override
        public void started() {
            if (state == STATE_PENDING_CONNECT) {
                sendConnectMessage();
            }
        }

        @Override
        public void stoped() {
            disconnected();
        }

        @Override
        public void received(String message) {
            if (message == null)
                return;

            lastMessageReceived = System.currentTimeMillis();
            String[] args = message.split(":");

            if (args == null || args.length == 0)
                return;

            if (args.length > 0 && "heartbeat".equals(args[0])) {
                return;
            }

            if (state == STATE_PENDING_CONNECT) {
                if (args.length > 2 && "connect".equals(args[0])) {
                    if ("accepted".equals(args[1])) {
                        if (playerName.equals(args[2])) {
                            state = STATE_CONNECTED;
                            if (listener != null) {
                                listener.connected();
                            }
                            return;
                        }
                    }
                }

                beginDisconnect();
            } else if ("disconnect".equals(args[0])) {
                beginDisconnect();
            } else {
                if (listener != null) {
                    listener.received(args);
                }
            }
        }
    };

    Runnable heartBeatRunnable = new Runnable() {
        @Override
        public void run() {
            while(state != STATE_DISCONNECTED) {
                long currentMillisec = System.currentTimeMillis();

                if (state == STATE_PENDING_CONNECT
                    && currentMillisec - connectingStarted > T_HEARTBEAT
                    && currentMillisec - connectingStarted < 4 * T_HEARTBEAT) {
                    sendConnectMessage();
                }
                else if (state == STATE_PENDING_CONNECT
                    && currentMillisec - connectingStarted >= 4 * T_HEARTBEAT) {
                    beginDisconnect();
                }
                else if (currentMillisec - lastMessageReceived > 2 * T_HEARTBEAT) {
                    beginDisconnect();
                }
                else {
                    if (currentMillisec - lastMessageSended > T_HEARTBEAT) {
                        sendHeartbeatMessage();
                    }
                }
            }
        }
    };

    public RemoteControllerPlayer(String host, int port, String playerName, RemoteControllerPlayerListener listener, boolean magneticSensor)
    {
        if (Utilities.empty(host)) {
            throw new IllegalArgumentException("host");
        }

        if (port < 0 || port >= 65536) {
            throw new IllegalArgumentException("port");
        }

        if (Utilities.empty(playerName)) {
            throw new IllegalArgumentException("playerName");
        }

        if (playerName.contains(":")) {
            throw new IllegalArgumentException("playerName");
        }

        this.remoteAddress = new InetSocketAddress(host, port);
        this.playerName = playerName;
        this.magneticSensor = magneticSensor;
        this.listener = listener;
        this.state = STATE_DISCONNECTED;
    }

    public boolean isConnectedOrPending() {
        return this.state != STATE_DISCONNECTED;
    }

    public String getPlayerName() {
        return this.playerName;
    }

    public boolean isMagneticSensorRequested() { return this.magneticSensor; }

    public void beginConnect() {
        synchronized (syncLock) {
            if (this.state == STATE_CONNECTED)
                return;

            if (this.state != STATE_DISCONNECTED)
                throw new IllegalStateException();

            if (channel != null) {
                throw new IllegalStateException();
            }

            if (channel == null) {
                state = STATE_PENDING_CONNECT;
                lastMessageReceived = connectingStarted = System.currentTimeMillis();
                channel = new RemoteControllerClientChannel(this.remoteAddress, this.mClientListener);
                channelThread = new Thread(channel);
                channelThread.start();
                heartBeatThread = new Thread(heartBeatRunnable);
                heartBeatThread.start();
            }
        }
    }

    public void beginDisconnect() {
        synchronized (syncLock) {
            if (this.state == STATE_DISCONNECTED)
                return;

            if (channel != null) {
                channel.beginClose("disconnect");
            }
        }
    }

    public void beginSendMessage(String message) {
        if (this.state != STATE_CONNECTED)
            return;

        sendMessage(message);
    }

    private void sendMessage(String message) {
        synchronized (syncLock) {
            if (channel != null) {
                channel.beginSend(message);
                lastMessageSended = System.currentTimeMillis();
            }
        }
    }

    private void sendConnectMessage() {
        sendMessage("connect:" + this.playerName);
    }

    private void sendHeartbeatMessage() {
        sendMessage("heartbeat");
    }

    private void disconnected() {
        synchronized (syncLock) {
            channelThread = null;
            channel = null;
            heartBeatThread = null;
            state = STATE_DISCONNECTED;

            if (listener != null) {
                listener.disconnected();
            }
        }
    }
}
