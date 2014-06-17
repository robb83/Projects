package com.simple.remotecontrollercar.map;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class RobotStates {
    public static final int UNINITIALIZED = 0;
    public static final int NONE = 1;
    public static final int MOVE = 2;

    public static final String[] Names = new String[]{
        "Uninitialized", "None", "Move", "Unknow"
    };

    public static String getStateName(int state)
    {
        if (state < 0 || state >= Names.length) return Names[Names.length - 1];
        return Names[state];
    }
}
