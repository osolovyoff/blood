#include "dxdevice.hxx"
#include "app.hxx"

DXDevice::DXDevice(){}
DXDevice::~DXDevice(){this->release();}

void DXDevice::initialize(HWND hwnd, int w, int h, bool is_fullscreen)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = w;
    sd.BufferDesc.Height = h;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 120;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = !is_fullscreen;

    m_driver_type = D3D_DRIVER_TYPE_HARDWARE;
    UINT createDeviceFlags = 0;
#if _DEBUG
    createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    D3D_FEATURE_LEVEL    featureLevel;
    IDXGISwapChain*      swap_chain = m_swap_chain.get();
    ID3D11Device*        device = m_device.get();
    ID3D11DeviceContext* immediate_contex = m_immediate_contex.get();


    HRESULT hr = D3D11CreateDeviceAndSwapChain
        ( nullptr
        , m_driver_type
        , nullptr
        , createDeviceFlags
        , feature_levels
        , _countof(feature_levels)
        , D3D11_SDK_VERSION
        , &sd
        , &swap_chain
        , &device
        , &featureLevel
        , &immediate_contex);
}

void DXDevice::release()
{
    if (m_immediate_contex)
    {
        m_immediate_contex->ClearState();
        m_immediate_contex.release();
    }
    if (m_render_target_view)
    {
        m_render_target_view->Release();
        m_render_target_view.release();
    }
    if (m_swap_chain)
    {
        m_swap_chain->Release();
        m_swap_chain.release();
    }
     
}

void DXDevice::render()
{

}
