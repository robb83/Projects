package com.simple.remoteshooter;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.Vibrator;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class ArenaActivity extends Activity {

    SensorManager sensorManager;
    PowerManager powerManager;
    PowerManager.WakeLock wakeLock;
    Sensor accelerometerSensor;
    Sensor magneticSensor;
    boolean paused = true;
    RemoteControllerService mService;
    boolean mBound = false;
    boolean subscribed;
    EditText editPlayerName;
    ProgressDialog progressBar;
    SensorEventListener currentSensorEventListener;
    Vibrator vibratorService;

    View.OnClickListener buttonResetClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            ArenaActivity.this.sendButtonEvent("c");
        }
    };

    View.OnClickListener buttonPauseClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            paused = !paused;

            ((Button)view).setText( paused ? getString(R.string.buttonPauseResume) : getString(R.string.buttonPausePause));

            ArenaActivity.this.sendButtonEvent("b");
        }
    };

    View.OnClickListener buttonActionClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            ArenaActivity.this.sendButtonEvent("a");
        }
    };

    View.OnClickListener buttonDisconnectClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            ArenaActivity.this.beginDisconnect();
        }
    };

    SensorEventListener acclerometerSensorListener = new SensorEventListener() {
        int requiredAccelerometerSamples = 3;
        float[] valuesAccelerometerValues = new float[3];
        float[] gravity = new float[3];
        int samplesAccelerometer = 0;
        float alpha = 0.8f;

        @Override
        public void onSensorChanged(SensorEvent sensorEvent) {

            if (sensorEvent.sensor.getType() != Sensor.TYPE_ACCELEROMETER)
                return;

            float[] values = sensorEvent.values;

            for(int i=0;i<3;i++)
            {
                gravity[i] = alpha * gravity[i] + (1 - alpha) * values[i];
                valuesAccelerometerValues[i] += (values[i] - gravity[i]) * -100;
            }

            ++samplesAccelerometer;

            if (samplesAccelerometer > requiredAccelerometerSamples) {

                for(int i=0;i<3;i++)
                {
                    valuesAccelerometerValues[i] = valuesAccelerometerValues[i] / (float)samplesAccelerometer;
                    if (valuesAccelerometerValues[i] > -5.0f && valuesAccelerometerValues[i] < 5.0f) valuesAccelerometerValues[i] = 0.0f;
                }

                if (Math.abs(valuesAccelerometerValues[0] + valuesAccelerometerValues[1] + valuesAccelerometerValues[2]) > 1) {
                    ArenaActivity.this.sendAccelerometerData(valuesAccelerometerValues);
                }

                valuesAccelerometerValues[0] = valuesAccelerometerValues[1] = valuesAccelerometerValues[2] = 0.0f;
                samplesAccelerometer = 0;
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int i) {

        }
    };

    SensorEventListener orientationSensorListener = new SensorEventListener() {
        int requiredAccelerometerSamples = 5, requiredMagneticSensorSamples = 1;
        float[] valuesAccelerometerValues = new float[3];
        float[] valuesMagneticFieldValues = new float[3];
        int samplesAccelerometer = 0, samplesMagneticSensor = 0;
        float[] matrixR = new float[9];
        float[] matrixValues = new float[3];

        @Override
        public void onSensorChanged(SensorEvent sensorEvent) {
            float[] values = sensorEvent.values;

            int sensorType = sensorEvent.sensor.getType();
            if (sensorType == Sensor.TYPE_ACCELEROMETER)
            {
                for(int i=0;i<3;i++)
                {
                    valuesAccelerometerValues[i] += values[i];
                }

                ++samplesAccelerometer;
            }
            else if (sensorType == Sensor.TYPE_MAGNETIC_FIELD)
            {
                for(int i=0;i<3;i++)
                {
                    valuesMagneticFieldValues[i] += values[i];
                }

                ++samplesMagneticSensor;
            }

            if (samplesAccelerometer > requiredAccelerometerSamples
                    && samplesMagneticSensor > requiredMagneticSensorSamples) {

                for(int i=0;i<3;i++)
                {
                    valuesAccelerometerValues[i] = valuesAccelerometerValues[i] / (float)samplesAccelerometer;
                    valuesMagneticFieldValues[i] = valuesMagneticFieldValues[i] / (float)samplesMagneticSensor;
                }

                boolean success = SensorManager.getRotationMatrix(
                        matrixR,
                        null /*matrixI*/,
                        valuesAccelerometerValues,
                        valuesMagneticFieldValues);

                if(success) {
                    SensorManager.getOrientation(matrixR, matrixValues);
                    ArenaActivity.this.sendDeviceOrientationData(matrixValues);
                }

                valuesAccelerometerValues[0] = valuesAccelerometerValues[1] = valuesAccelerometerValues[2] = 0.0f;
                valuesMagneticFieldValues[0] = valuesMagneticFieldValues[1] = valuesMagneticFieldValues[2] = 0.0f;
                samplesAccelerometer = samplesMagneticSensor = 0;
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int i) {

        }
    };

    ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            RemoteControllerService.LocalBinder binder = (RemoteControllerService.LocalBinder) service;
            mService = binder.getService();
            if (mService != null) {
                mService.addPlayerListener(mPlayerListener);

                accelerometerSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
                if (accelerometerSensor == null)
                {
                    Toast toast = Toast.makeText(ArenaActivity.this, "Accelerometer sensor not found.", Toast.LENGTH_LONG);
                    toast.show();

                    finish();
                    return;
                }

                if (mService.isMagneticSensorRequested())
                {
                    magneticSensor = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
                    if (magneticSensor == null)
                    {
                        Toast toast = Toast.makeText(ArenaActivity.this, "Magnetic field sensor not found.", Toast.LENGTH_LONG);
                        toast.show();

                        finish();
                        return;
                    }
                }

                if (mService.isMagneticSensorRequested())
                    currentSensorEventListener = orientationSensorListener;
                else
                    currentSensorEventListener = acclerometerSensorListener;

                if (editPlayerName != null) {
                    editPlayerName.setText(mService.getPlayerName());
                }

                subscribe();
            }

            mBound = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName arg0) {
            if (mService != null)
                mService.removePlayerListener(mPlayerListener);
            mBound = false;
        }
    };

    RemoteControllerPlayerListener mPlayerListener = new RemoteControllerPlayerListener() {
        @Override
        public void connected() {
        }

        @Override
        public void disconnected() {
            if (progressBar != null) {
                progressBar.dismiss();
            }
            finish();
        }

        @Override
        public void received(String[] message) {
            if (message == null || message.length == 0)
                return;

            if (message.length > 1) {
                if ("a".equals(message[0]) && "a".equals(message[1])) {
                    if (vibratorService != null) {
                        vibratorService.vibrate(100);
                    }
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_arena);

        powerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        wakeLock = powerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "com.simple.remoteshooter.MainActivity");

        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        vibratorService = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);

        Button button;

        button = (Button)findViewById(R.id.buttonReset);
        if (button != null)
            button.setOnClickListener(buttonResetClickListener);

        button = (Button)findViewById(R.id.buttonAction1);
        if (button != null)
            button.setOnClickListener(buttonActionClickListener);

        button = (Button)findViewById(R.id.buttonPause);
        if (button != null)
            button.setOnClickListener(buttonPauseClickListener);

        button = (Button)findViewById(R.id.buttonDisconnect);
        if (button != null)
            button.setOnClickListener(buttonDisconnectClickListener);

        editPlayerName = (EditText)findViewById(R.id.editPlayerName);
        if (editPlayerName != null) {
            editPlayerName.setEnabled(false);
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        Intent intent = new Intent(this, RemoteControllerService.class);
        bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (mService != null) {
            mService.beginDisconnect();
        }
        if (mBound) {
            unbindService(mConnection);
            mBound = false;
        }
    }

    @Override
    protected void onPause() {
        if (wakeLock != null) wakeLock.release();
        unsubscribe();
        super.onPause();
    }

    @Override
    protected void onResume() {
        if (wakeLock != null) wakeLock.acquire();
        subscribe();
        super.onResume();
    }

    @Override
    public void onBackPressed() {
        if (!beginDisconnect()) {
            super.onBackPressed();
        }
    }

    void sendButtonEvent(String buttonName) {
        sendCommand("a:" + buttonName);
    }

    void sendAccelerometerData(float[] values) {
        if (paused)
            return;

        sendCommand("b:" + values[0] + ":" + values[1] + ":" + values[2]);
    }

    void sendDeviceOrientationData(float[] values) {
        if (paused)
            return;

        sendCommand("c:" + values[0] + ":" + values[1] + ":" + values[2]);
    }

    void sendCommand(String command) {
        if (command == null || command.length() == 0)
            return;

        if (mService != null) {
            mService.beginSend(command);
        }
    }

    boolean beginDisconnect() {
        if (mService != null && mService.isConnected()) {

            progressBar = new ProgressDialog(this);
            progressBar.setCancelable(false);
            progressBar.setMessage("Disconnecting ...");
            progressBar.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progressBar.show();

            mService.beginDisconnect();

            return true;
        }

        return false;
    }

    void subscribe() {
        synchronized (this) {

            if (subscribed)
                return;

            if (sensorManager != null && currentSensorEventListener != null) {
                if (accelerometerSensor != null) {
                    sensorManager.registerListener(currentSensorEventListener, accelerometerSensor, SensorManager.SENSOR_DELAY_FASTEST);
                }

                if (magneticSensor != null) {
                    sensorManager.registerListener(currentSensorEventListener, magneticSensor, SensorManager.SENSOR_DELAY_FASTEST);
                }

                subscribed = true;
            }
        }
    }

    void unsubscribe() {
        synchronized (this) {
            subscribed = false;
            if (sensorManager != null && currentSensorEventListener != null) {
                sensorManager.unregisterListener(currentSensorEventListener);
            }
        }
    }

    public static void open(Context context) {
        Intent intent = new Intent(context, ArenaActivity.class);
        context.startActivity(intent);
    }
}
