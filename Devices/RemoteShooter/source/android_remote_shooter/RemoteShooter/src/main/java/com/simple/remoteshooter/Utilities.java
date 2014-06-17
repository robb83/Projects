package com.simple.remoteshooter;

/**
 * Created by robb83@gmail.com on 2014.05.23..
 */
public class Utilities {

    public final static boolean DEBUG = true;

    public static boolean empty(String value)
    {
        if (value == null)
            return true;

        value = value.trim();

        return value.length() == 0;
    }

}
