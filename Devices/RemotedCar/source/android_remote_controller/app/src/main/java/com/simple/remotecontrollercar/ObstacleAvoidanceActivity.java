package com.simple.remotecontrollercar;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;

import com.simple.remotecontrollercar.view.CircularSectorView;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class ObstacleAvoidanceActivity extends Activity {

    private Button m_buttonClose;
    private CircularSectorView m_circularSectorView;
    private BluetoothService m_bluetoothService;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.obstacle_avoidance);

        m_buttonClose = (Button)findViewById(R.id.buttonClose);
        m_buttonClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                finish();
            }
        });

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
                ObstacleAvoidanceActivity.this.runOnUiThread(m_disconnectedRunnable);
            }

            @Override
            public void DataReceived(byte[] message) {

            }

            private final Runnable m_disconnectedRunnable = new Runnable() {
                @Override
                public void run() {
                    ObstacleAvoidanceActivity.this.finish();
                }
            };
        });
        if (m_bluetoothService.isClosed())
        {
            finish();
        } else if (m_bluetoothService.getState() == BluetoothService.STATE_CONNECTED)
        {
            m_bluetoothService.sendMessage("o1");
        }
    }
}