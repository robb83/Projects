package com.simple.remotecontrollercar.map;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class RobotMoveData {
    public long Timestamp;
    public float LeftTicks;
    public float LeftSpeed;
    public float LeftDirection;
    public float RightTicks;
    public float RightSpeed;
    public float RightDirection;

    public float getSignedLeftTicks()
    {
        return (LeftDirection == 1 ? LeftTicks : -LeftTicks);
    }

    public float getSignedRightTicks()
    {
        return (RightDirection == 1 ? RightTicks : -RightTicks);
    }
}
