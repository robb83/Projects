package com.simple.remotecontrollercar;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.Button;

import com.simple.remotecontrollercar.math.Calculator;
import com.simple.remotecontrollercar.view.CircularSectorView;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class AccelerometerStearingActivity extends Activity {

    private CircularSectorView m_circularSectorView;
    private BluetoothService m_bluetoothService;
    private SensorManager m_sensorManager;
    private Sensor m_accelerometerSensor;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);

        if (m_sensorManager == null)
        {
            finish();
        }

        m_accelerometerSensor = m_sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        if (m_accelerometerSensor == null)
        {
            finish();
        }

        m_circularSectorView = new CircularSectorView(this, null);
        m_circularSectorView.setTouchEnabled(false);
        m_circularSectorView.addListener(new CircularSectorView.ISelectionChanged() {
            @Override
            public void changed(int sector) {
                switch (sector) {
                    case CircularSectorView.SECTOR_UP:
                        m_bluetoothService.sendMessage("b");
                        break;
                    case CircularSectorView.SECTOR_UP_RIGHT:
                        break;
                    case CircularSectorView.SECTOR_RIGHT:
                        m_bluetoothService.sendMessage("g");
                        break;
                    case CircularSectorView.SECTOR_DOWN_RIGHT:
                        break;
                    case CircularSectorView.SECTOR_DOWN:
                        m_bluetoothService.sendMessage("c");
                        break;
                    case CircularSectorView.SECTOR_DOWN_LEFT:
                        break;
                    case CircularSectorView.SECTOR_LEFT:
                        m_bluetoothService.sendMessage("f");
                        break;
                    case CircularSectorView.SECTOR_UP_LEFT:
                        break;
                    case CircularSectorView.SECTOR_CENTER:
                    default:
                        m_bluetoothService.sendMessage("d");
                        break;
                }
            }
        });
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(m_circularSectorView);

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
                AccelerometerStearingActivity.this.runOnUiThread(m_disconnectedRunnable);
            }

            @Override
            public void DataReceived(byte[] message) {

            }

            private final Runnable m_disconnectedRunnable = new Runnable() {
                @Override
                public void run() {
                AccelerometerStearingActivity.this.finish();
                }
            };
        });
        if (m_bluetoothService.isClosed())
        {
            finish();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (m_sensorManager != null)
        {
            m_sensorManager.registerListener(m_circularSectorView,
                    m_accelerometerSensor,
                    SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();

        if (m_sensorManager != null)
        {
            m_sensorManager.unregisterListener(m_circularSectorView);
        }
    }
}