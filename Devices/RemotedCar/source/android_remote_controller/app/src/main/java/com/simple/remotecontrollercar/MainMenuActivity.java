package com.simple.remotecontrollercar;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.Toast;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class MainMenuActivity extends Activity {
    private Button m_buttonManualStearing;
    private Button m_buttonExactStearing;
    private Button m_buttonAccelerometerStearing;
    private Button m_buttonVoiceStearing;
    private Button m_buttonObstacleAvoidance;
    private Button m_buttonConnect;
    private BluetoothService m_bluetoothService;
    private WatchdogThread m_watWatchdogThread;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);

        m_bluetoothService = BluetoothService.instance;
        m_bluetoothService.addListener(new IBluetoothServiceListener() {
            @Override
            public void Connected(String deviceName) {
                MainMenuActivity.this.runOnUiThread(m_connectedRunnable);
            }

            @Override
            public void Connecting(String deviceName) {
                MainMenuActivity.this.runOnUiThread(m_connectingRunnable);
            }

            @Override
            public void Distonnected(String deviceName) {
                MainMenuActivity.this.runOnUiThread(m_disconnectedRunnable);
            }

            @Override
            public void DataReceived(byte[] message) {

            }

            private Runnable m_connectedRunnable = new Runnable() {
                @Override
                public void run() {
                    MainMenuActivity.this.onConnected();
                }
            };

            private Runnable m_disconnectedRunnable = new Runnable() {
                @Override
                public void run() {
                    MainMenuActivity.this.onDisconnected();
                }
            };

            private Runnable m_connectingRunnable = new Runnable() {
                @Override
                public void run() {
                    MainMenuActivity.this.onConnecting();
                }
            };
        });

        setContentView(R.layout.mainmenu);

        m_buttonObstacleAvoidance = (Button)findViewById(R.id.ButtonObstacleAvoidance);
        m_buttonManualStearing = (Button)findViewById(R.id.ButtonManualStearing);
        m_buttonExactStearing = (Button)findViewById(R.id.ButtonExactStearing);
        m_buttonVoiceStearing = (Button)findViewById(R.id.ButtonVoiceStearing);
        m_buttonAccelerometerStearing = (Button)findViewById(R.id.ButtonAccelerometerStearing);
        m_buttonConnect = (Button)findViewById(R.id.ButtonConnect);

        m_buttonManualStearing.setEnabled(false);
        m_buttonExactStearing.setEnabled(false);
        m_buttonAccelerometerStearing.setEnabled(false);
        m_buttonVoiceStearing.setEnabled(false);
        m_buttonObstacleAvoidance.setEnabled(false);

        m_buttonManualStearing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent activity = new Intent(MainMenuActivity.this, ManualStearingActivity.class);
                startActivity(activity);
            }
        });
        m_buttonExactStearing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent activity = new Intent(MainMenuActivity.this, ExactStearingActivity.class);
                startActivity(activity);
            }
        });
        m_buttonAccelerometerStearing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent activity = new Intent(MainMenuActivity.this, AccelerometerStearingActivity.class);
                startActivity(activity);
            }
        });
        m_buttonVoiceStearing.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent activity = new Intent(MainMenuActivity.this, VoiceStearingActivity.class);
                startActivity(activity);
            }
        });
        m_buttonObstacleAvoidance.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent activity = new Intent(MainMenuActivity.this, ObstacleAvoidanceActivity.class);
                startActivity(activity);
            }
        });
        m_buttonConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (m_bluetoothService.isClosed())
                {
                    if (!m_bluetoothService.isBluetoothSupported())
                    {
                        Toast.makeText(MainMenuActivity.this,
                                "Bluetooth is not available",
                                Toast.LENGTH_LONG).show();
                    } else if(!m_bluetoothService.isBluetoothEnabled())
                    {
                        Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(enableIntent, BluetoothService.REQUEST_ENABLE_BT);
                    } else {
                        Intent serverIntent = new Intent(MainMenuActivity.this, DeviceListActivity.class);
                        startActivityForResult(serverIntent, BluetoothService.REQUEST_CONNECT_DEVICE);
                    }
                } else {
                    m_bluetoothService.disconnect();
                }
            }
        });

        m_watWatchdogThread = new WatchdogThread();
        m_watWatchdogThread.start();
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (m_bluetoothService.getState() == BluetoothService.STATE_CONNECTED)
        {
            m_bluetoothService.sendMessage("n");
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (m_watWatchdogThread != null)
        {
            m_watWatchdogThread.cancel();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case BluetoothService.REQUEST_CONNECT_DEVICE:
                if (resultCode == Activity.RESULT_OK) {

                    if (data == null) break;

                    Bundle bundle = data.getExtras();
                    if (bundle == null) break;

                    String address = bundle.getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                    m_bluetoothService.connect(address);
                }
                break;
            case BluetoothService.REQUEST_ENABLE_BT:
                if (resultCode == Activity.RESULT_OK)
                {
                    Intent serverIntent = new Intent(this, DeviceListActivity.class);
                    startActivityForResult(serverIntent, BluetoothService.REQUEST_CONNECT_DEVICE);
                } else {
                    Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                }
                break;
        }
    }

    void onConnected()
    {
        m_buttonManualStearing.setEnabled(true);
        m_buttonExactStearing.setEnabled(true);
        m_buttonAccelerometerStearing.setEnabled(true);
        m_buttonVoiceStearing.setEnabled(true);
        m_buttonObstacleAvoidance.setEnabled(true);
        m_buttonConnect.setEnabled(true);
        m_buttonConnect.setText("Disconnect");
        m_buttonConnect.setBackgroundResource(R.drawable.button_red);
    }

    void onDisconnected()
    {
        m_buttonManualStearing.setEnabled(false);
        m_buttonExactStearing.setEnabled(false);
        m_buttonAccelerometerStearing.setEnabled(false);
        m_buttonVoiceStearing.setEnabled(false);
        m_buttonObstacleAvoidance.setEnabled(false);
        m_buttonConnect.setEnabled(true);
        m_buttonConnect.setText("Connect");
        m_buttonConnect.setBackgroundResource(R.drawable.button_green);
    }

    void onConnecting()
    {
        m_buttonConnect.setEnabled(false);
        m_buttonConnect.setBackgroundResource(R.drawable.button_blue);
        m_buttonConnect.setText("Connecting...");
    }

    class WatchdogThread extends Thread {

        private volatile boolean m_pendingClose;

        @Override
        public void run() {
            BluetoothService bluetoothService = BluetoothService.instance;

            while(!m_pendingClose)
            {
                if (bluetoothService.getState() == BluetoothService.STATE_CONNECTED)
                {
                    bluetoothService.sendMessage("e");
                }

                try {
                    for(int i=0;i<30;i++)
                    {
                        Thread.sleep(100);

                        if (m_pendingClose) break;
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        public void cancel()
        {
            m_pendingClose = true;
        }
    }
}