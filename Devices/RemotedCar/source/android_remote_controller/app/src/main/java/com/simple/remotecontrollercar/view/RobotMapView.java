package com.simple.remotecontrollercar.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import com.simple.remotecontrollercar.map.Robot;
import com.simple.remotecontrollercar.map.RobotMap;
import com.simple.remotecontrollercar.map.RobotStates;
import com.simple.remotecontrollercar.math.Vector2;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class RobotMapView extends SurfaceView implements SurfaceHolder.Callback  {

    public final int RED = Color.RED;
    public final int GREEN = Color.GREEN;
    public final int YELLOW = Color.YELLOW;
    public final int ROBOT_RADIUS = 10;
    public final int CENTIMETER_TO_PIXEL = 1;

    private RobotMap m_map;
    private Paint m_robotPainter;
    private Paint m_robotHeadPainter;
    private Paint m_targetPointPainter;
    private int m_ox;
    private int m_oy;

    public RobotMapView(Context context) {
        super(context);

        initialize();
    }

    private void initialize()
    {
        getHolder().addCallback(this);
        setFocusable(true);

        m_robotPainter = new Paint();
        m_robotPainter.setColor(RED);
        m_robotPainter.setStyle(Paint.Style.FILL);

        m_robotHeadPainter = new Paint();
        m_robotHeadPainter.setColor(GREEN);
        m_robotHeadPainter.setStyle(Paint.Style.FILL);

        m_targetPointPainter = new Paint();
        m_targetPointPainter.setColor(YELLOW);
        m_targetPointPainter.setStyle(Paint.Style.FILL);

        m_ox = (this.getWidth() / 2);
        m_oy = (this.getHeight() / 2);
    }

    public Vector2 setScreenToWorld(Vector2 vec)
    {
        vec.setX((vec.getX() - m_ox));
        vec.setY((vec.getY() - m_oy));

        return vec;
    }

    public void setWorldToScreen(Vector2 vec)
    {
        vec.setX((vec.getX() + m_ox) * CENTIMETER_TO_PIXEL);
        vec.setY((vec.getY() + m_oy) * CENTIMETER_TO_PIXEL);
    }

    public float getOriginX()
    {
        return m_ox;
    }

    public float getOriginY()
    {
        return m_oy;
    }

    public RobotMap getMap()
    {
        return m_map;
    }

    public void setMap(RobotMap map)
    {
        m_map = map;
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        Log.d("TRACE", "onSizeChanged");

        super.onSizeChanged(w, h, oldw, oldh);

        m_ox = (w / 2);
        m_oy = (h / 2);
    }

    Vector2 convertToScreen(Vector2 vec)
    {
        return new Vector2(vec.getX() + m_ox, vec.getY() + m_oy);
    }

    public void render(Canvas canvas)
    {
        Vector2 pos1, pos2, pos3;
        if (canvas == null) return;

        canvas.drawColor(Color.BLACK);

        if (m_map == null) return;

        Robot robot = m_map.getRobot();

        if (robot != null)
        {
            pos1 = robot.getPosition();
            pos3 = robot.getHeadingVector().clone();
            pos3.multiple(10);

            pos1 = convertToScreen(pos1);

            while (pos1.getX() < 0) pos1.setX(pos1.getX() + this.getWidth());
            while (pos1.getX() > this.getWidth()) pos1.setX(pos1.getX() - this.getWidth());
            while (pos1.getY() < 0) pos1.setY(pos1.getY() + this.getHeight());
            while (pos1.getY() > this.getHeight()) pos1.setY(pos1.getY() - this.getHeight());

            pos3.add(pos1);

            canvas.drawCircle(pos1.getX(), pos1.getY(), ROBOT_RADIUS, m_robotPainter);
            canvas.drawLine(pos1.getX(), pos1.getY(), pos3.getX(), pos3.getY(), m_robotHeadPainter);

            pos1 = m_map.getTargetPoint();
            if (pos1 != null)
            {
                pos1 = convertToScreen(pos1);

                canvas.drawCircle(pos1.getX(), pos1.getY(), ROBOT_RADIUS, m_targetPointPainter);
            }

            String state = RobotStates.getStateName(robot.getState());
            canvas.drawText(state, 5, 15, m_robotHeadPainter);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        Log.d("TRACE", "surfaceCreated");
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i2, int i3) {
        Log.d("TRACE", "surfaceChanged");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        Log.d("TRACE", "surfaceDestoryed");
    }
}
