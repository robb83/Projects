package com.simple.remoteshooter;

/**
 * Created by robb83@gmail.com on 2014.05.23..
 */
public interface RemoteControllerClientListener {
    void started();

    void stoped();

    void received(String message);
}
