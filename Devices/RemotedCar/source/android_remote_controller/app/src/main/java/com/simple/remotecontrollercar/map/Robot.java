package com.simple.remotecontrollercar.map;

import android.util.Log;

import com.simple.remotecontrollercar.math.Calculator;
import com.simple.remotecontrollercar.math.Vector2;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class Robot {
    private Vector2 m_position;
    private Vector2 m_side;
    private float m_heading;
    private Vector2 m_headingVector;
    private float m_wheel_diameter;
    private float m_wheel_width;
    private float m_encoder_ticks_per_revolution;
    private float m_axis_width;
    private float m_axis_distance;
    private float m_body_width;
    private float m_body_height;
    private float m_wheel_circumference;
    private float m_distancePerTicks;
    private int m_state;

    public boolean waiter;

    public Robot(float wheel_diameter, float wheel_width,
                 float encoder_ticks_per_revolution,
                 float axis_width, float axis_distance,
                 float body_width, float body_height)
    {
        m_wheel_diameter = wheel_diameter;
        m_wheel_width = wheel_width;
        m_encoder_ticks_per_revolution = encoder_ticks_per_revolution;
        m_axis_distance = axis_distance;
        m_axis_width = axis_width;
        m_body_height = body_height;
        m_body_width = body_width;

        m_position = new Vector2(0, 0);
        m_heading = (float)Math.toRadians(270);
        m_headingVector = Vector2.polarToCertesian(1.0f, m_heading);
        m_side = m_headingVector.prep();

        m_wheel_circumference = (float)(Math.PI * m_wheel_diameter);
        m_distancePerTicks = m_wheel_circumference / m_encoder_ticks_per_revolution;
    }

    public float getEncoderTicksPerRevolution()
    {
        return m_encoder_ticks_per_revolution;
    }

    public Vector2 getSide()
    {
        return m_side;
    }

    public int getState()
    {
        return m_state;
    }

    public void setState(int state)
    {
        m_state = state;
        waiter = false;
    }

    public float getHeading()
    {
        return m_heading;
    }

    public Vector2 getHeadingVector()
    {
        return m_headingVector;
    }

    public Vector2 getPosition()
    {
        return m_position;
    }

    public void move(RobotMoveData data)
    {
        // ez akkor nem jó, ha mozgatják alatta a földet
        if (data.LeftSpeed + data.RightSpeed == 0) return;

        // ticks to cm
        float leftDistance = data.getSignedLeftTicks() * m_distancePerTicks;
        float rightDistance = data.getSignedRightTicks() * m_distancePerTicks;

        // displacement
        float distance = (rightDistance + leftDistance) / 2;
        float offsetHeading = (rightDistance - leftDistance) / (m_axis_width * 2);

        m_heading = Calculator.normalizeRadian(m_heading - offsetHeading);
        m_headingVector = Vector2.polarToCertesian(1.0f, m_heading);
        m_position.add((float) (distance * (Math.cos(m_heading))), (float) (distance * (Math.sin(m_heading))));
        m_side = m_headingVector.prep();

        Log.d("ROBOT", "Move heading: " + Math.toDegrees(m_heading));
    }

    @Override
    public String toString() {
        return String.format("R[X: %f, Y: %f, H: %f]", m_position.getX(), m_position.getY(), Math.toDegrees(m_heading));
    }
}
