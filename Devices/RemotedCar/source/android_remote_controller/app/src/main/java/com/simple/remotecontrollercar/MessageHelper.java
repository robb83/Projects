package com.simple.remotecontrollercar;

import com.simple.remotecontrollercar.map.Robot;
import com.simple.remotecontrollercar.map.RobotMoveData;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class MessageHelper
{
    public static long ParseInt(byte[] buffer, int offset, int end)
    {
        boolean isNegative = false;
        long value = 0;
        byte c;

        while(offset < end)
        {
            c = buffer[offset++];

            if(c == '-')
            {
                isNegative = true;
            } else if(c >= '0' && c <= '9')
            {
                value = value * 10 + c - '0';
            } else {
                break;
            }
        }

        if(isNegative) return -value;

        return value;
    }

    public static int IndexOf(byte[] buffer, int offset, int end, byte target)
    {
        while(offset < end)
        {
            if ( target == buffer[offset]) return offset;

            ++offset;
        }

        return -1;
    }

    public static Robot createOrUpdateRobot(byte[] message, int offset, Robot robot)
    {
        float a,b,c,d,e,f,g;
        a = (float)MessageHelper.ParseInt(message, offset, message.length) / 10; // mm to cm
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        b = (float)MessageHelper.ParseInt(message, offset, message.length) / 10; // mm to cm
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        c = (float)MessageHelper.ParseInt(message, offset, message.length);
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        d = (float)MessageHelper.ParseInt(message, offset, message.length) / 10; // mm to cm
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        e = (float)MessageHelper.ParseInt(message, offset, message.length) / 10; // mm to cm
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        f = (float)MessageHelper.ParseInt(message, offset, message.length) / 10; // mm to cm
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        g = (float)MessageHelper.ParseInt(message, offset, message.length) / 10; // mm to cm

        if (robot == null)
        {
            robot = new Robot(a,b,c,d,e,f,g);
        }

        return robot;
    }

    public static RobotMoveData readRobotMoveData(byte[] message, int offset)
    {
        RobotMoveData moveData = new RobotMoveData();

        moveData.LeftTicks = MessageHelper.ParseInt(message, offset, message.length);
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        moveData.RightTicks = MessageHelper.ParseInt(message, offset, message.length);
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        moveData.LeftSpeed = MessageHelper.ParseInt(message, offset, message.length);
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        moveData.RightSpeed = MessageHelper.ParseInt(message, offset, message.length);
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        moveData.LeftDirection = MessageHelper.ParseInt(message, offset, message.length);
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        moveData.RightDirection = MessageHelper.ParseInt(message, offset, message.length);
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        // x
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        // y
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        // heading
        offset = MessageHelper.IndexOf(message, offset, message.length, (byte)':') + 1;
        moveData.Timestamp = MessageHelper.ParseInt(message, offset, message.length);

        return moveData;
    }

    /*
    public static long ParseInt(ByteBuffer buffer)
    {
        boolean isNegative = false;
        long value = 0;

        int c = PeekNextDigit(buffer);

        if (c == -1) return 0;

        do{
            if(c == '-')
                isNegative = true;
            else if(c >= '0' && c <= '9')
                value = value * 10 + c - '0';

            buffer.get();

            c = Peek(buffer);
        }
        while( c >= 0 && (c >= '0' && c <= '9'));

        if(isNegative) return -value;
        return value;
    }   */
      /*
    public static float ParseFloat(ByteBuffer buffer)
    {
        boolean isNegative = false;
        boolean isFraction = false;
        long value = 0;
        float fraction = 1.0f;

        char c = (char)PeekNextDigit(buffer);
        // ignore non numeric leading characters
        if(c < 0)
            return 0; // zero returned if timeout

        do{
            if(c == '-')
                isNegative = true;
            else if (c == '.')
                isFraction = true;
            else if(c >= '0' && c <= '9')  {      // is c a digit?
                value = value * 10 + c - '0';
                if(isFraction)
                    fraction *= 0.1;
            }

            buffer.get();
            c = (char)Peek(buffer);

        }
        while( (c >= '0' && c <= '9')  || c == '.');

        if(isNegative)
            value = -value;
        if(isFraction)
            return value * fraction;
        else
            return value;
    }

    // Get next valid digit byte or -1
    static int PeekNextDigit(ByteBuffer buffer)
    {
        int c;
        c = Peek(buffer);
        if (c == '-') return c;
        if (c >= '0' && c <= '9') return c;

        return -1;
    }

    // Read next byte from buffer without change current position
    static int Peek(ByteBuffer buffer)
    {
        if (buffer.remaining()>0)
        {
            int position = buffer.position();
            byte[] array = buffer.array();
            return array[position];
        }
        return -1;
    }*/

}
