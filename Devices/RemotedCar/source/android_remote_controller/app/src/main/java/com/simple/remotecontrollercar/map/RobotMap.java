package com.simple.remotecontrollercar.map;

import android.util.Log;
import com.simple.remotecontrollercar.math.Calculator;
import com.simple.remotecontrollercar.math.Vector2;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class RobotMap {
    private Robot m_robot;
    private Vector2 m_targetPoint;

    public RobotMap()
    {
    }

    public Robot getRobot()
    {
        return m_robot;
    }

    public void setRobot(Robot robot)
    {
        m_robot = robot;
    }

    public Vector2 getTargetPoint()
    {
        return m_targetPoint;
    }

    public void setTargetPoint(Vector2 target)
    {
        m_targetPoint = target;
    }
}
