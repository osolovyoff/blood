#pragma once
#ifndef BLOOD_DXCAMERA
#define BLOOD_DXCAMERA

class Camera
{
public:
    void initialize();

    void yaw(float angle);
    void roll(float angle);
    void pitch(float angle);

    void strafe(float value);
    void up(float value);

    void move_up(float value);
    void move_down(float value);
    void move_left(float value);
    void move_right(float value);

    void move_forward(float value);
    void move_back(float value);

private:

private:
    bool m_is_moved;
};

#endif // BLOOD_DXCAMERA