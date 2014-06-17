package com.simple.remoteshooter;

/**
 * Created by robb83@gmail.com on 2014.05.23..
 */
public interface RemoteControllerPlayerListener {
    void connected();

    void disconnected();

    void received(String[] message);
}
