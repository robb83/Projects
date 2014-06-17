package com.simple.remotecontrollercar;

import android.os.Environment;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.logging.Logger;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class RobotLogger {
    static String s_outputFileName;
    static String s_externalWritableDirectory;

    static
    {
        s_externalWritableDirectory = Environment.getExternalStorageDirectory().getAbsolutePath() + "/download/";
        GenerateNewFile();
    }

    public static void GenerateNewFile()
    {
        s_outputFileName = "" + System.currentTimeMillis();
    }

    public static void Log(String message)
    {
        if (message == null || message.length() == 0) return;

        Log(message.getBytes());
    }

    public synchronized static void Log(byte[] message)
    {
        if (message == null || message.length == 0) return;

        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            FileOutputStream out = null;
            try {
                out = new FileOutputStream(s_externalWritableDirectory + s_outputFileName + ".txt", true);
                out.write(message);
                out.flush();
            } catch (FileNotFoundException e1) {
                e1.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (out != null) {
                    try {
                        out.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
            }
        } else {
            Log.i(s_outputFileName, new String(message));
        }
    }
}
