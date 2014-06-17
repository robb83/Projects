package com.simple.remotecontrollercar.math;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class Calculator {
    public static final float PI = (float)Math.PI;
    public static final float PI2 = (float)(Math.PI * 2);
    public static final float PIper2 = (float)(Math.PI / 2);
    public static final float PIper4 = (float)(Math.PI / 4);

    public static float normalizeRadian(float value)
    {
        while(value > PI2) value -= PI2;
        while(value < 0) value += PI2;

        return value;
    }

    public static float constrain(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    public static float sign(float value)
    {
        if (value < 0) return -1;
        if (value > 0) return 1;
        return 0;
    }
}
