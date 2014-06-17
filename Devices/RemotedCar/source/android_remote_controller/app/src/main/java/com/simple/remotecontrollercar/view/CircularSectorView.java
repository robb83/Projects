package com.simple.remotecontrollercar.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import java.util.ArrayList;
import java.util.List;

import com.simple.remotecontrollercar.math.Calculator;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class CircularSectorView extends View implements SensorEventListener {

    public static final int SECTOR_UP = 6;
    public static final int SECTOR_UP_RIGHT = 7;
    public static final int SECTOR_RIGHT = 0;
    public static final int SECTOR_DOWN_RIGHT = 1;
    public static final int SECTOR_DOWN = 2;
    public static final int SECTOR_DOWN_LEFT = 3;
    public static final int SECTOR_LEFT = 4;
    public static final int SECTOR_UP_LEFT = 5;
    public static final int SECTOR_CENTER = 8;

    private List<ISelectionChanged> m_changeListeners = new ArrayList<ISelectionChanged>();
    private Paint m_line = new Paint();
    private Paint m_fill = new Paint();
    private Paint m_clear = new Paint();
    private int m_selected = SECTOR_CENTER;
    private boolean m_touch_enabled = true;
    private int m_sensor_x;
    private int m_sensor_y;
    private long m_sensor_timestamp;
    private float m_sensor_gravity_filter[] = new float[3];

    public CircularSectorView(Context context, AttributeSet attrs)
    {
        super(context, attrs);

        m_line.setColor(0xffffffff);
        m_line.setStyle(Paint.Style.STROKE);
        m_line.setStrokeWidth(2);
        m_line.setAntiAlias(true);
        m_line.setTextSize(16);

        m_fill.setColor(0xffff0000);
        m_fill.setStyle(Paint.Style.FILL);

        m_clear.setColor(0xff000000);
        m_clear.setStyle(Paint.Style.FILL);
    }

    public boolean getTouchEnabled()
    {
        return m_touch_enabled;
    }

    public void setTouchEnabled(boolean touch)
    {
        m_touch_enabled = touch;
    }

    public void addListener(ISelectionChanged listener)
    {
        synchronized (m_changeListeners)
        {
            if (!m_changeListeners.contains(listener))
                m_changeListeners.add(listener);
        }
    }

    public void removeListener(ISelectionChanged listener)
    {
        synchronized (m_changeListeners)
        {
            m_changeListeners.remove(listener);
        }
    }

    public void setSelect(int sector)
    {
        if (sector == m_selected) return;

        if (sector >= 0 && sector <= SECTOR_CENTER)
        {
            m_selected = sector;
            if (sector == CircularSectorView.SECTOR_UP_LEFT) {
                m_sensor_y = -1;
                m_sensor_x = -1;
            } else if (sector == CircularSectorView.SECTOR_UP) {
                m_sensor_y = -1;
                m_sensor_x = 0;
            } else if (sector == CircularSectorView.SECTOR_UP_RIGHT) {
                m_sensor_y = -1;
                m_sensor_x = 1;
            } else if (sector == CircularSectorView.SECTOR_LEFT) {
                m_sensor_y = 0;
                m_sensor_x = -1;
            } else if (sector == CircularSectorView.SECTOR_CENTER) {
                m_sensor_y = 0;
                m_sensor_x = 0;
            } else if (sector == CircularSectorView.SECTOR_RIGHT) {
                m_sensor_y = 0;
                m_sensor_x = 1;
            } else if (sector == CircularSectorView.SECTOR_DOWN_LEFT) {
                m_sensor_y = 1;
                m_sensor_x = -1;
            } else if (sector == CircularSectorView.SECTOR_DOWN) {
                m_sensor_y = 1;
                m_sensor_x = 0;
            } else if (sector == CircularSectorView.SECTOR_DOWN_RIGHT) {
                m_sensor_y = 1;
                m_sensor_x = 1;
            }
            invalidate();

            synchronized (m_changeListeners){
                for(ISelectionChanged l : m_changeListeners)
                {
                    l.changed(m_selected);
                }
            }
        }
    }

    public int getSelect()
    {
        return m_selected;
    }

    @Override
    public boolean performClick() {
        return true;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {

        if (m_touch_enabled == false) return true;

        if ((event.getActionMasked() & MotionEvent.ACTION_UP) == 0) return true;

        int width = getWidth();
        int height = getHeight();
        int centerx = width/2;
        int centery = height/2;
        int r1 = width/2;
        int r2 = width/4;

        float x = event.getX();
        float y = event.getY();

        if (x < 0 || x >= width) return false;
        if (y < 0 || y >= height) return false;

        float x1 = (x - centerx);
        float y1 = -(y - centery);

        float length = (float)(Math.sqrt(x1 * x1 + y1 * y1));

        if (length <= r2)
        {
            setSelect(SECTOR_CENTER);
        } else if (length <= r1)
        {
            // 0 - 360 + 22.5 = 382.5
            double angle = Math.atan2(y1, x1);

            angle = Math.toDegrees(angle * -1);

            while(angle<0) angle += 360;
            while(angle>360) angle -= 360;

            angle += (45.0 / 2);

            while(angle<0) angle += 360;
            while(angle>360) angle -= 360;

            int index = (int)(angle / 45);
            setSelect(index);
        }

        return true;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        int width = getWidth();
        int height = getHeight();
        int centerx = width/2;
        int centery = height/2;
        int r1 = width/2;
        int r2 = width/4;

        float rad45 =  (float)Math.PI / 4;
        float rad22 = rad45 / 2;
        float pi2 = (float)(Math.PI * 2);

        if (m_selected == SECTOR_CENTER)
        {
            canvas.drawCircle(centerx, centery, r2, m_fill);
        } else if (m_selected >= 0 && m_selected < SECTOR_CENTER)
        {
            RectF oval = new RectF(
                centerx - r1,
                centery - r1,
                centerx + r1,
                centery + r1
            );

            canvas.drawArc(oval, (float)Math.toDegrees((m_selected * rad45)-rad22), 45, true, m_fill);
            canvas.drawCircle(centerx, centery, r2, m_clear);
        }

        // outer
        canvas.drawCircle(centerx, centery, r1, m_line);
        // inner
        canvas.drawCircle(centerx, centery, r2, m_line);

        for(float angle = -rad22;angle < pi2 - rad22;angle += rad45)
        {
            int starx = (int)(r2 * Math.cos(angle));
            int starty =  (int)(r2 * Math.sin(angle));

            int endx =(int)(r1 * Math.cos(angle));
            int endy = (int)(r1 * Math.sin(angle));

            canvas.drawLine(centerx + starx, centery + starty, centerx + endx, centery + endy, m_line);
        }
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {

        final float alpha = 0.8f;

        m_sensor_gravity_filter[0] = alpha * m_sensor_gravity_filter[0] + (1 - alpha) * sensorEvent.values[0];
        m_sensor_gravity_filter[1] = alpha * m_sensor_gravity_filter[1] + (1 - alpha) * sensorEvent.values[1];
        // m_sensor_gravity_filter[2] = alpha * m_sensor_gravity_filter[2] + (1 - alpha) * sensorEvent.values[2];

        float x = sensorEvent.values[0] - m_sensor_gravity_filter[0];
        float y = sensorEvent.values[1] - m_sensor_gravity_filter[1];
        //float z = sensorEvent.values[2] - m_sensor_gravity_filter[2];

        // if (System.currentTimeMillis() - m_sensor_timestamp < 300 ) return; // 1.5 sec
        // m_sensor_timestamp = System.currentTimeMillis();

        x = ((x / sensorEvent.sensor.getMaximumRange()) * 100) * -1;
        y = (y / sensorEvent.sensor.getMaximumRange()) * 100;

        if ((x > 3 && x < 5) || (x < -3 && x > -5))
        {
            x = Calculator.sign(x);
        } else {
            x = 0;
        }

        if ((y > 3 && y < 5) || (y < -3 && y > -5))
        {
            y = Calculator.sign(y);
        } else
        {
            y = 0;
        }

        m_sensor_x = (int)Calculator.constrain(m_sensor_x + x, -1, 1);
        m_sensor_y = (int)Calculator.constrain(m_sensor_y + y, -1, 1);

        if (m_sensor_y == -1 && m_sensor_x == -1)
        {
            setSelect(CircularSectorView.SECTOR_UP_LEFT);
        } else if (m_sensor_y == -1 && m_sensor_x == 0)
        {
            setSelect(CircularSectorView.SECTOR_UP);
        } else if (m_sensor_y == -1 && m_sensor_x == 1)
        {
            setSelect(CircularSectorView.SECTOR_UP_RIGHT);
        } else if (m_sensor_y == 0 && m_sensor_x == -1)
        {
            setSelect(CircularSectorView.SECTOR_LEFT);
        } else if (m_sensor_y == 0 && m_sensor_x == 0)
        {
            setSelect(CircularSectorView.SECTOR_CENTER);
        } else if (m_sensor_y == 0 && m_sensor_x == 1)
        {
            setSelect(CircularSectorView.SECTOR_RIGHT);
        } else if (m_sensor_y == 1 && m_sensor_x == -1)
        {
            setSelect(CircularSectorView.SECTOR_DOWN_LEFT);
        } else if (m_sensor_y == 1 && m_sensor_x == 0)
        {
            setSelect(CircularSectorView.SECTOR_DOWN);
        } else if (m_sensor_y == 1 && m_sensor_x == 1)
        {
            setSelect(CircularSectorView.SECTOR_DOWN_RIGHT);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    public interface ISelectionChanged
    {
        void changed(int sector);
    }
}
