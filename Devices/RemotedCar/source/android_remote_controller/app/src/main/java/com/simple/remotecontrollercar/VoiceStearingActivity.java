package com.simple.remotecontrollercar;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.speech.RecognizerIntent;
import android.speech.SpeechRecognizer;
import android.util.Log;

import java.util.ArrayList;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class VoiceStearingActivity extends Activity {

    private static final int SPEECH_REQUEST_ID = 1;
    SpeechRecognizer m_speechRecognizer;
    private BluetoothService m_bluetoothService;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!SpeechRecognizer.isRecognitionAvailable(this))
        {
            finish();
        }

        m_speechRecognizer = SpeechRecognizer.createSpeechRecognizer(this);

        if (m_speechRecognizer == null)
        {
            finish();
        }

        m_bluetoothService = BluetoothService.instance;

        m_bluetoothService.addListener(new IBluetoothServiceListener() {
            @Override
            public void Connecting(String deviceName) {

            }

            @Override
            public void Connected(String deviceName) {

            }

            @Override
            public void Distonnected(String deviceName) {
                VoiceStearingActivity.this.runOnUiThread(m_disconnectedRunnable);
            }

            @Override
            public void DataReceived(byte[] message) {

            }

            private final Runnable m_disconnectedRunnable = new Runnable() {
                @Override
                public void run() {
                VoiceStearingActivity.this.finish();
                }
            };
        });

        if (m_bluetoothService.isClosed())
        {
            finish();
        }

        recognizeCommand();
    }

    void recognizeCommand()
    {
        Intent speech = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);

        speech.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.EXTRA_LANGUAGE_MODEL);
        speech.putExtra(RecognizerIntent.EXTRA_PROMPT, "Say: \"Előre\",\"Hátra\",\"Jobbra\",\"Balra\",\"Állj\",\"Stop\"");

        startActivityForResult(speech, SPEECH_REQUEST_ID);
    }

    void processCommands(ArrayList<String> commands)
    {
        if (commands != null)
        {
            if (startWith("kil", commands))
            {
                finish();
            } else if (startWith("sto", commands) || startWith("szto", commands) || startWith("á", commands))
            {
                m_bluetoothService.sendMessage("d");
            } else if (startWith("elő", commands) || startWith("elö", commands))
            {
                m_bluetoothService.sendMessage("b");
            } else if (startWith("hát", commands))
            {
                m_bluetoothService.sendMessage("c");
            } else if (startWith("job", commands))
            {
                m_bluetoothService.sendMessage("g");
            } else if (startWith("bal", commands))
            {
                m_bluetoothService.sendMessage("f");
            } else
            {
                m_bluetoothService.sendMessage("d");
            }
        }

        recognizeCommand();
    }

    boolean startWith(String prefix, ArrayList<String> stringList)
    {
        for(String s : stringList)
        {
            if (s.startsWith(prefix)) return true;
        }

        return false;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        if (requestCode == SPEECH_REQUEST_ID)
        {
            Log.d("ROBOT", "" + resultCode);

            if (resultCode == -1)
            {
                ArrayList<String> results = data.getExtras().getStringArrayList(RecognizerIntent.EXTRA_RESULTS);
                processCommands(results);
            } else {
                finish();
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
    }
}