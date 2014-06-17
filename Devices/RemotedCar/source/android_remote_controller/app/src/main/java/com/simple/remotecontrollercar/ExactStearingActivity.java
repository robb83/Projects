package com.simple.remotecontrollercar;

import android.app.Activity;
import android.graphics.Canvas;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.widget.Button;

import com.simple.remotecontrollercar.map.Robot;
import com.simple.remotecontrollercar.map.RobotMap;
import com.simple.remotecontrollercar.map.RobotMoveData;
import com.simple.remotecontrollercar.math.Calculator;
import com.simple.remotecontrollercar.math.Vector2;
import com.simple.remotecontrollercar.view.CircularSectorView;
import com.simple.remotecontrollercar.view.RobotMapView;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class ExactStearingActivity extends Activity {

    private BluetoothService m_bluetoothService;
    private RobotMap m_robotMap;
    private RobotMapView m_robotMapView;
    private volatile boolean m_running = true;

    private final Thread m_mainThread = new Thread() {
        private final static int MAX_FPS = 25;
        private final static int FRAME_PERIOD = 1000 / MAX_FPS;

        @Override
        public void run() {
            long beginTime, timeDiff, sleepTime;
            SurfaceHolder holder = m_robotMapView.getHolder();
            firstDraw();
            while(m_running)
            {
                beginTime = System.currentTimeMillis();

                robotUpdate();
                robotDraw(holder);

                timeDiff = System.currentTimeMillis() - beginTime;
                sleepTime = (int)(FRAME_PERIOD - timeDiff);
                if (sleepTime > 0) {
                    try {
                        Thread.sleep(sleepTime);
                    } catch (InterruptedException ignored) {}
                }
            }
        }

        void firstDraw()
        {
            if (m_bluetoothService.getState() == BluetoothService.STATE_CONNECTED)
            {
                m_bluetoothService.sendMessage("n");
                m_bluetoothService.sendMessage("l");
                m_bluetoothService.sendMessage("a");
            }
        }

        void robotUpdate()
        {
            if (m_robotMap == null) return;

            Robot robot = m_robotMap.getRobot();
            Vector2 targetPoint = m_robotMap.getTargetPoint();

            if (robot == null || targetPoint  == null) return;

            if (robot.getState() != 1) return;
            if (robot.waiter) return;

            Vector2 direction = targetPoint.clone();
            direction.sub(robot.getPosition());

            int distance = (int)direction.length();
            if (distance < 5) return;

            double angle = Math.atan2(direction.getY(), direction.getX());

            int head;

            if (angle < 0)
            {
                head = (int)Math.toDegrees((Math.PI - (angle * -1)) + Math.PI);
            }
            else
            {
                head = (int)Math.toDegrees(angle);
            }

            int rotate = ((int)Math.toDegrees(robot.getHeading())) - head;

            if (rotate <= -10 || rotate >= 10)
            {
                Log.d("ROBOT", "Head: " + Math.toDegrees(robot.getHeading()));
                Log.d("ROBOT", "Angle: " + Math.toDegrees(angle));
                Log.d("ROBOT", "Orientation: " + head);
                Log.d("ROBOT", "Diff: " + rotate);

                robot.waiter = true;

                if (rotate < 0)
                {
                    m_bluetoothService.sendMessage("k" + (rotate * -1));
                } else {
                    m_bluetoothService.sendMessage("j" + rotate);
                }
            } else
            {
                int d = distance;
                        //(int)(distance * robot.getEncoderTicksPerRevolution() * 10);

                if (d <= 0) return;
                if (d > 5000) d = 5000;
                if (d > 100) d = 100;
                robot.waiter = true;
                m_bluetoothService.sendMessage("h" + d);
            }
        }

        void robotDraw(SurfaceHolder holder)
        {
            Canvas canvas = null;
            try {
                canvas = holder.lockCanvas();
                synchronized (holder) {
                    m_robotMapView.render(canvas);
                }
            } finally {
                if (canvas != null) {
                    holder.unlockCanvasAndPost(canvas);
                }
            }
        }
    };

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_robotMap = new RobotMap();
        m_robotMapView = new RobotMapView(this);
        m_robotMapView.setMap(m_robotMap);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(m_robotMapView);

        m_robotMapView.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if ((motionEvent.getActionMasked() & MotionEvent.ACTION_UP) == 0) return true;

                float x = motionEvent.getX();
                float y = motionEvent.getY();

                if (x < 0 || x > view.getWidth()) return false;
                if (y < 0 || y > view.getHeight()) return false;

                Vector2 targetPoint = new Vector2(motionEvent.getX(), motionEvent.getY());
                targetPoint = m_robotMapView.setScreenToWorld(targetPoint);

                 m_robotMap.setTargetPoint(targetPoint);

                return true;
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
                ExactStearingActivity.this.runOnUiThread(m_disconnectedRunnable);
            }

            @Override
            public void DataReceived(byte[] message) {
                if (message == null || message.length == 0) return;
                // RobotLogger.Log(message);

                int offset = 0;
                byte type = message[offset++];

                if (type == 'C')
                {
                    m_robotMap.setRobot(MessageHelper.createOrUpdateRobot(message, offset, m_robotMap.getRobot()));
                } else if (type == 'D')
                {
                    Robot robot = m_robotMap.getRobot();

                    if (robot != null)
                    {
                        long state = MessageHelper.ParseInt(message, offset, message.length);
                        robot.setState((int)state);
                    }
                } else if (type == 'E')
                {
                    Robot robot = m_robotMap.getRobot();

                    if (robot != null)
                    {
                        RobotMoveData data = MessageHelper.readRobotMoveData(message, offset);

                        if (data != null)
                        {
                            robot.move(data);
                        }
                    }
                }
            }

            private final Runnable m_disconnectedRunnable = new Runnable() {
                @Override
                public void run() {
                    ExactStearingActivity.this.finish();
                }
            };
        });
        if (m_bluetoothService.isClosed())
        {
            finish();
        }
    }

    @Override
    protected void onStart() {
        super.onStart();

        if (!m_mainThread.isAlive())
        {
            m_mainThread.start();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();

        m_running = false;
        try {
            m_mainThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}