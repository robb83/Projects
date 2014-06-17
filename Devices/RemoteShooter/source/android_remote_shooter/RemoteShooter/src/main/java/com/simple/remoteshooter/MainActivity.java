package com.simple.remoteshooter;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.IBinder;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.Toast;
import android.content.SharedPreferences;

public class MainActivity extends Activity {

    public final String SettingsName = "com.simple.remoteshooter.connectionSettings";
    public final String SettingsRemoteAddress = "remoteAddress";
    public final String SettingsRemotePort = "remotePort";
    public final String SettingsPlayerName = "playerName";

    EditText editHostAddress;
    EditText editHostPort;
    EditText editPlayerName;
    RadioButton rbOrientationSensor;
    RadioButton rbAccelerationSensor;
    RemoteControllerService mService;
    boolean mBound = false;
    ProgressDialog progressBar;

    View.OnClickListener buttonConnectClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {

            boolean acceleroMeter = (rbAccelerationSensor != null && rbAccelerationSensor.isEnabled() && rbAccelerationSensor.isChecked());
            boolean orinetationSensor = (rbOrientationSensor != null && rbOrientationSensor.isEnabled() && rbOrientationSensor.isChecked());

            if (acceleroMeter == false && orinetationSensor == false)
            {
                Toast notSupportedToast = Toast.makeText(MainActivity.this, getString(R.string.textNotSupportedSensor), Toast.LENGTH_SHORT);
                notSupportedToast.show();

                return;
            }

            SharedPreferences settings = getSharedPreferences(SettingsName, MODE_PRIVATE);
            SharedPreferences.Editor editor = settings.edit();

            editor.putString(SettingsRemoteAddress, editHostAddress.getText().toString());
            editor.putString(SettingsRemotePort, editHostPort.getText().toString());
            editor.putString(SettingsPlayerName, editPlayerName.getText().toString());
            editor.commit();

            connect(editHostAddress.getText().toString(),
                    editHostPort.getText().toString(),
                    editPlayerName.getText().toString(),
                    orinetationSensor);
        }
    };

    private RemoteControllerPlayerListener mPlayerListener = new RemoteControllerPlayerListener() {
        @Override
        public void connected() {
            if (progressBar != null)
                progressBar.dismiss();

            ArenaActivity.open(MainActivity.this);
        }

        @Override
        public void disconnected() {
            if (progressBar != null)
                progressBar.dismiss();
        }

        @Override
        public void received(String[] message) {

        }
    };

    private ServiceConnection mConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName className, IBinder service) {
            RemoteControllerService.LocalBinder binder = (RemoteControllerService.LocalBinder) service;
            mService = binder.getService();
            if (mService != null)
                mService.addPlayerListener(mPlayerListener);
            mBound = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName arg0) {
            if (mService != null)
                mService.removePlayerListener(mPlayerListener);
            mBound = false;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        SensorManager sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        boolean hasAccelerometerSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) != null;
        boolean hasMagneticFieldSensor = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD) != null;

        Button button;

        button = (Button)findViewById(R.id.buttonConnect);
        if (button != null)
            button.setOnClickListener(buttonConnectClickListener);

        editHostAddress = (EditText)findViewById(R.id.editHostAddress);
        editHostPort = (EditText)findViewById(R.id.editHostPort);
        editPlayerName = (EditText)findViewById(R.id.editPlayerName);

        rbAccelerationSensor = (RadioButton)findViewById(R.id.rbAcceleration);
        if (rbAccelerationSensor != null) {
            rbAccelerationSensor.setEnabled(hasAccelerometerSensor);
        }

        rbOrientationSensor = (RadioButton)findViewById(R.id.rbOrientation);
        if (rbOrientationSensor != null) {
            rbOrientationSensor.setEnabled(hasMagneticFieldSensor);
        }

        SharedPreferences settings = getSharedPreferences(SettingsName, MODE_PRIVATE);
        editHostAddress.setText(settings.getString(SettingsRemoteAddress, "192.168.0.12"));
        editHostPort.setText(settings.getString(SettingsRemotePort, "5545"));
        editPlayerName.setText(settings.getString(SettingsPlayerName, "robb83@gmail.com"));
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
        if (mBound) {
            unbindService(mConnection);
            mBound = false;
        }
    }

    private void connect(String hostAddress, String hostPort, String playerName, boolean magneticSensor)
    {
        if (Utilities.empty(hostAddress) || Utilities.empty(hostPort) || Utilities.empty(playerName))
        {
            Toast toast = Toast.makeText(this, getString(R.string.textRequiredFields), Toast.LENGTH_LONG);
            toast.show();
            return;
        }

        int port = Integer.parseInt(hostPort);

        if (mBound) {

            progressBar = new ProgressDialog(this);
            progressBar.setCancelable(true);
            progressBar.setMessage("Connecting ...");
            progressBar.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            progressBar.setOnCancelListener(new DialogInterface.OnCancelListener() {
                @Override
                public void onCancel(DialogInterface dialogInterface) {
                    if (mService != null)
                    {
                        mService.beginDisconnect();
                    }
                }
            });
            progressBar.show();

            try {
                mService.beginConnect(hostAddress, port, playerName, magneticSensor);
            }
            catch (Exception e)
            {
                e.printStackTrace();

                progressBar.dismiss();
            }
        }
    }
}
