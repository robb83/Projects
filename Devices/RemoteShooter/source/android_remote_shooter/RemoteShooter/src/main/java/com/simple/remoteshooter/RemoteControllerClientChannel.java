package com.simple.remoteshooter;

import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.LinkedList;

/**
 * Created by robb83@gmail.com on 2014.05.23..
 */
public class RemoteControllerClientChannel implements Runnable {
    boolean running, pendingClose;
    InetSocketAddress remoteAddress;
    LinkedList<byte[]> pendingPackets = new LinkedList<byte[]>();
    RemoteControllerClientListener listener;
    Exception lastException;
    int sendableTotalPackets = 0;

    public RemoteControllerClientChannel(InetSocketAddress remoteAddress, RemoteControllerClientListener listener)
    {
        this.pendingClose = false;
        this.remoteAddress = remoteAddress;
        this.listener = listener;
    }

    public void beginClose() {
        this.pendingClose = true;
    }

    public void beginClose(String message) {

        if (this.pendingClose)
            return;

        this.pendingClose = true;

        byte[] data;
        if (message == null)
            data = null;
        else
            data = message.getBytes();

        synchronized (pendingPackets) {
            pendingPackets.clear();
            if (data != null)
                pendingPackets.addLast(data);
            sendableTotalPackets = pendingPackets.size();
        }
    }

    public void beginSend(String message) {
        if (this.pendingClose)
            return;

        if (message == null)
            return;

        byte[] data = message.getBytes();

        synchronized (pendingPackets)
        {
            pendingPackets.addLast(data);
            sendableTotalPackets = pendingPackets.size();
        }
    }

    public Exception getLastException()
    {
        return this.lastException;
    }

    public boolean isRunning() {
        return this.running;
    }

    public boolean isPendingClose() {
        return this.pendingClose;
    }

    @Override
    public void run() {
        final int MAX_PACKET_PER_ITERATION = 10;
        DatagramChannel channel;
        SocketAddress sourceAddress;
        ByteBuffer readerByteBuffer;
        ByteBuffer writerByteBuffer;
        LinkedList<byte[]> temporaryPacketBuffer;
        byte[] buffer;
        byte[] temporaryData;

        try {
            this.running = true;

            if (listener != null)
            {
                listener.started();
            }

            temporaryPacketBuffer = new LinkedList<byte[]>();
            buffer = new byte[16 * 1024];
            readerByteBuffer = ByteBuffer.wrap(buffer);
            readerByteBuffer.clear();
            writerByteBuffer = ByteBuffer.wrap(buffer);
            writerByteBuffer.clear();

            channel = DatagramChannel.open();
            channel.configureBlocking(false);

            channel.connect(this.remoteAddress);

            while(!this.pendingClose || (this.pendingClose && sendableTotalPackets > 0))
            {
                if (channel.read(readerByteBuffer) > 0)
                {
                    String message = new String(buffer, 0, readerByteBuffer.position());
                    if (listener != null)
                    {
                        listener.received(message);
                    }

                    readerByteBuffer.clear();
                }

                synchronized (this.pendingPackets)
                {
                    int packets = this.pendingPackets.size();

                    for(int i=0; i < MAX_PACKET_PER_ITERATION && i < packets; i++)
                    {
                        temporaryPacketBuffer.addLast(this.pendingPackets.removeFirst());
                    }

                    sendableTotalPackets = this.pendingPackets.size();
                }

                if (temporaryPacketBuffer.size() > 0)
                {
                    while(temporaryPacketBuffer.size() > 0)
                    {
                        temporaryData = temporaryPacketBuffer.removeFirst();

                        writerByteBuffer.clear();
                        writerByteBuffer.put(temporaryData);
                        writerByteBuffer.flip();
                        channel.write(writerByteBuffer);
                        temporaryData = null;
                    }
                }

                if (sendableTotalPackets > 0) {
                    Thread.sleep(50);
                } else {
                    Thread.sleep(150);
                }
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            this.lastException = e;
        }
        finally{
            this.running = false;

            if (listener != null)
            {
                listener.stoped();
            }
        }
    }
}
