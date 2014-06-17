package com.simple.remotecontrollercar;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class BluetoothService {

    // Intent request codes
    public static final int REQUEST_CONNECT_DEVICE = 1;
    public static final int REQUEST_ENABLE_BT = 2;

    public static final int STATE_NONE = 0;
    public static final int STATE_CONNECTING = 1;
    public static final int STATE_CONNECTED = 2;

    private final String NAME = "BluetoothCommunicationService";
    private final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    public static final BluetoothService instance = new BluetoothService();

    private List<IBluetoothServiceListener> m_bluetoothListeners = new ArrayList<IBluetoothServiceListener>();
    private BluetoothAdapter m_bluetoothAdapter;
    private int m_state;
    private ClientThread m_clientThread;

    private BluetoothService()
    {
        m_state = STATE_NONE;
        m_bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    public void addListener(IBluetoothServiceListener listener)
    {
        synchronized (m_bluetoothListeners)
        {
            if (!m_bluetoothListeners.contains(listener))
                m_bluetoothListeners.add(listener);
        }
    }

    public void removeListener(IBluetoothServiceListener listener)
    {
        synchronized (m_bluetoothListeners)
        {
            m_bluetoothListeners.remove(listener);
        }
    }

    public int getState()
    {
        return m_state;
    }

    public boolean isBluetoothSupported()
    {
        return m_bluetoothAdapter != null;
    }

    public boolean isBluetoothEnabled()
    {
        return m_bluetoothAdapter.isEnabled();
    }

    public boolean isClosed()
    {
        return m_state == STATE_NONE;
    }

    public void connect(String address)
    {
        connect(m_bluetoothAdapter.getRemoteDevice(address));
    }

    public void connect(BluetoothDevice device)
    {
        if (m_clientThread != null)
        {
            m_clientThread.cancel();
        }

        if (device == null)
        {
            SetState(STATE_NONE);
            return;
        }

        m_clientThread = new ClientThread(device);
        m_clientThread.start();

        SetState(STATE_CONNECTING);
        onConnecting(device.getName());
    }

    public void disconnect()
    {
        if (m_clientThread != null)
        {
            m_clientThread.cancel();
        }

        SetState(STATE_NONE);
    }

    public void sendMessage(String message)
    {
        if (message == null) return;
        sendMessage(message.getBytes());
    }

    public void sendMessage(byte[] message)
    {
        if (m_state == STATE_CONNECTED)
        {
            m_clientThread.write(message);
        }
    }

    private void SetState(int state)
    {
        m_state = state;
    }

    private void onConnected(String deviceName)
    {
        synchronized (m_bluetoothListeners){
            for(IBluetoothServiceListener l : m_bluetoothListeners)
            {
                l.Connected(deviceName);
            }
        }
    }

    private void onDisconnected(String deviceName)
    {
        synchronized (m_bluetoothListeners){
            for(IBluetoothServiceListener l : m_bluetoothListeners)
            {
                l.Distonnected(deviceName);
            }
        }
    }

    private void onConnecting(String deviceName)
    {
        synchronized (m_bluetoothListeners){
            for(IBluetoothServiceListener l : m_bluetoothListeners)
            {
                l.Connecting(deviceName);
            }
        }
    }

    private void onMessageReceived(byte[] message)
    {
        synchronized (m_bluetoothListeners){
            for(IBluetoothServiceListener l : m_bluetoothListeners)
            {
                l.DataReceived(message);
            }
        }
    }

    class ClientThread extends Thread
    {
        private final BluetoothDevice m_device;
        private BluetoothSocket m_socket;
        private InputStream m_inputStream;
        private OutputStream m_outputStream;

        public ClientThread(BluetoothDevice device) {
            m_device = device;
        }

        @Override
        public void run() {
            // for more speed
            m_bluetoothAdapter.cancelDiscovery();

            // create/connect
            try {
                m_socket = m_device.createRfcommSocketToServiceRecord(MY_UUID);
                m_socket.connect();
            } catch (IOException e) {

                Log.e(NAME, "create() failed", e);

                SetState(STATE_NONE);
                onDisconnected(m_device.getName());

                try {
                    m_socket.close();
                } catch (IOException e2) {
                    Log.e(NAME, "unable to close() socket during connection failure", e2);
                }

                return;
            }

            // create stream
            try {
                m_inputStream = m_socket.getInputStream();
                m_outputStream = m_socket.getOutputStream();
            } catch (IOException e) {
                Log.e(NAME, "temp sockets not created", e);

                SetState(STATE_NONE);
                onDisconnected(m_device.getName());

                return;
            }

            SetState(STATE_CONNECTED);
            onConnected(m_device.getName());

            ByteBuffer buffer = ByteBuffer.allocate(8 * 1024);
            byte[] array = buffer.array();
            int readed, offset = 0, start, end;

            // read
            while (true) {
                try {
                    readed = m_inputStream.read(array, offset, array.length - offset);
                    start = 0;
                    end = -1;

                    while(true)
                    {
                        // search message close symbol
                        for(int i=start;i < offset + readed; i++)
                        {
                            if (array[i] == '\n')
                            {
                                end = i;
                                break;
                            }
                        }

                        if (end == -1)
                        {
                            // no message close symbol
                            offset = offset + readed;

                            // buffer full
                            if (offset + 1 == array.length)
                            {
                                throw new IOException();
                            }

                            if (start > 0)
                            {
                                System.arraycopy(array, start, array, 0, offset - start);
                                offset = offset - start;
                            }

                            break;
                        } else {
                            // create message
                            byte[] message = new byte[ end - start + 1 ];
                            System.arraycopy(array, start, message, 0, end - start + 1);

                            onMessageReceived(message);

                            start = end + 1; // next symbol
                            end = -1;
                        }
                    }

                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                } catch(IOException e2)
                {
                    Log.e(NAME, "disconnected", e2);
                    SetState(STATE_NONE);
                    onDisconnected(m_device.getName());
                    break;
                }
            }
        }

        public void write(byte[] data)
        {
            try {
                Log.i(NAME, "Send: " + new String(data));
                m_outputStream.write(data);
            } catch (IOException e) {
                Log.e(NAME, "Exception during write", e);
            }
        }

        public void cancel()
        {
            try {
                m_socket.close();
            } catch (IOException e) {
                Log.e(NAME, "close() of connect socket failed", e);
            }
        }
    }
}
