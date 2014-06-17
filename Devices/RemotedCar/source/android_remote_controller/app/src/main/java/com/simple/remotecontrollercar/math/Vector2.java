package com.simple.remotecontrollercar.math;

/**
 * Created by robb83@gmail.com on 2013.06.02..
 */
public class Vector2 {
    private float m_x;
    private float m_y;

    public Vector2()
    {

    }

    public Vector2(float x, float y)
    {
        m_x = x;
        m_y = y;
    }

    public float getX() { return m_x; }
    public void setX(float v) { m_x = v;}

    public float getY() { return m_y; }
    public void setY(float v) { m_y = v;}

    public void add(Vector2 vec)
    {
        m_x += vec.m_x;
        m_y += vec.m_y;
    }

    public void sub(Vector2 vec)
    {
        m_x -= vec.m_x;
        m_y -= vec.m_y;
    }

    public void add(float x, float y)
    {
        m_x += x;
        m_y += y;
    }

    public void multiple(float value)
    {
        m_x *= value;
        m_y *= value;
    }

    public float angle(Vector2 vec)
    {
        return (float)Math.atan2(vec.m_y - m_y, vec.m_x - m_x);
    }

    public float distanceSq(Vector2 vec)
    {
        float dx = vec.m_x - m_x;
        float dy = vec.m_y - m_y;

        return dx * dx + dy * dy;
    }

    public float distance(Vector2 vec)
    {
        float dx = vec.m_x - m_x;
        float dy = vec.m_y - m_y;

        return (float)Math.sqrt(dx * dx + dy * dy);
    }

    public float length()
    {
        return (float)(Math.sqrt(m_x * m_x + m_y * m_y));
    }

    public void normalize()
    {
        float length = (float)Math.sqrt(m_x * m_x + m_y * m_y);

        if (length < 1) return;

        m_x = m_x / length;
        m_y = m_y / length;
    }

    public Vector2 prep()
    {
        return new Vector2(-m_y, m_x);
    }

    public float dot(Vector2 vec)
    {
        return m_x * vec.m_x + m_y * vec.m_y;
    }

    public Vector2 clone()
    {
        return new Vector2(m_x, m_y);
    }

    public Vector2 rotate(float angle)
    {
        double
            sin = Math.sin(angle),
            cos = Math.cos(angle);

        return new Vector2(
            (float)(m_x * cos - m_y * sin),
            (float)(m_x * sin + m_y * cos));
    }

    public static Vector2 polarToCertesian(float r, float angleInRadian)
    {
        return new Vector2(
                (float)(r * Math.cos(angleInRadian)),
                (float)(r * Math.sin(angleInRadian)));
    }

    public static Vector2 transformToLocal(Vector2 point, Vector2 origin, Vector2 head, Vector2 side)
    {
        return new Vector2(
                (float)(point.m_x * head.m_x + point.m_y * head.m_y) - origin.dot(head),
                (float)(point.m_x * side.m_x + point.m_y * side.m_y) - origin.dot(side));
    }

    public static Vector2 transformToWorld(Vector2 point, Vector2 origin, Vector2 head, Vector2 side)
    {
        return new Vector2(
                (float)(point.m_x * head.m_x - point.m_y * head.m_y) + origin.m_x,
                (float)(point.m_x * side.m_x + point.m_y * side.m_y) + origin.m_y);
    }
}
