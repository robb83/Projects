package com.simple.remoteshooter;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;

import java.util.LinkedList;

/**
 * Created by robb83@gmail.com on 2014.05.23..
 */
public class RemoteControllerService extends Service {

    public class LocalBinder extends Binder {
        RemoteControllerService getService() {
            return RemoteControllerService.this;
        }
    }

    private final IBinder mBinder = new LocalBinder();
    private RemoteControllerPlayer mPlayer;
    private LinkedList<RemoteControllerPlayerListener> mPlayerListeners = new LinkedList<RemoteControllerPlayerListener>();
    private RemoteControllerPlayerListener mPlayerListener = new RemoteControllerPlayerListener() {
        @Override
        public void connected() {
            synchronized (mPlayerListeners) {
                for (RemoteControllerPlayerListener listener : mPlayerListeners)
                    listener.connected();
            }
        }

        @Override
        public void disconnected() {
            synchronized (mPlayerListeners) {
                for (RemoteControllerPlayerListener listener : mPlayerListeners)
                    listener.disconnected();
            }
        }

        @Override
        public void received(String[] message) {
            synchronized (mPlayerListeners) {
                for (RemoteControllerPlayerListener listener : mPlayerListeners)
                    listener.received(message);
            }
        }
    };

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        if (mPlayer != null)
        {
            try {
                mPlayer.beginDisconnect();
            } catch (Exception e) { }
        }
    }

    public boolean isConnected() {
        return mPlayer == null ? false : mPlayer.isConnectedOrPending();
    }

    public void beginConnect(String host, int port, String playerName, boolean magneticSensor) {
        if (mPlayer != null && mPlayer.isConnectedOrPending())
        {
            throw new IllegalStateException();
        }

        mPlayer = new RemoteControllerPlayer(host, port, playerName, mPlayerListener, magneticSensor);
        mPlayer.beginConnect();
    }

    public String getPlayerName() {

        if (mPlayer == null)
            return null;

        return mPlayer.getPlayerName();
    }

    public boolean isMagneticSensorRequested() {

        if (mPlayer == null)
            return false;

        return mPlayer.isMagneticSensorRequested();
    }

    public void beginDisconnect() {
        if (mPlayer != null) {
            mPlayer.beginDisconnect();
        }
    }

    public void beginSend(String message) {
        if (mPlayer != null) {
            mPlayer.beginSendMessage(message);
        }
    }

    public void addPlayerListener(RemoteControllerPlayerListener listener) {
        synchronized (mPlayerListeners) {
            if (!mPlayerListeners.contains(listener))
                mPlayerListeners.addLast(listener);
        }
    }

    public void removePlayerListener(RemoteControllerPlayerListener listener) {
        synchronized (mPlayerListeners) {
            mPlayerListeners.remove(listener);
        }
    }
}
