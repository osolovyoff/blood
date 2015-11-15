#pragma once
#ifndef BLOOD_DXCAMERA
#define BLOOD_DXCAMERA

#ifdef USE_DIRECTX

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx9math.h>

class Camera
{
public:
    Camera();
    void set_view_params(D3DXVECTOR3* eye, D3DXVECTOR3* lockat);
    void set_projection_params(float fov, float aspect, float near_plane, float far_plane);

    /*void yaw(float angle);
    void roll(float angle);
    void pitch(float angle);

    void strafe(float value);
    void up(float value);

    void move_up(float value);
    void move_down(float value);
    void move_left(float value);
    void move_right(float value);

    void move_forward(float value);
    void move_back(float value);*/

public:
    const D3DXMATRIX*  get_view_matrix() const;
    const D3DXMATRIX*  get_proj_matrix() const;
    const D3DXVECTOR3* get_eye() const;

private:

    bool m_is_moved;
};

#endif // BLOOD_DXCAMERA
#endif // USE_DIRECTX