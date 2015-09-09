#pragma once
#include <memory>
#include <d3d11.h>
#include <D3DX11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

class DXDevice
{
public:
    DXDevice();
    ~DXDevice();

    void initialize(HWND, int w, int h, bool is_fullscreen);
    void release();

    void render();

private:
    HWND                                    m_hwnd;
    HINSTANCE                               m_hinstance;
    D3D_DRIVER_TYPE                         m_driver_type;
    std::unique_ptr<ID3D11Device>           m_device;
    std::unique_ptr<ID3D11DeviceContext>    m_immediate_contex;

    std::unique_ptr<IDXGISwapChain>         m_swap_chain;
    std::unique_ptr<ID3D11RenderTargetView> m_render_target_view;
};