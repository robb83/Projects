package com.simple.remotecontrollercar;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public interface IBluetoothServiceListener {
    void Connecting(String deviceName);
    void Connected(String deviceName);
    void Distonnected(String deviceName);
    void DataReceived(byte[] message);
}
