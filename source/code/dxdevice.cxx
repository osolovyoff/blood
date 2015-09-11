#include "dxdevice.hxx"
#include "app.hxx"

DXDevice::DXDevice()
: m_device(NULL)
, m_immediate_contex(NULL)
, m_render_target_view(NULL)
, m_swap_chain(NULL)
{}
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

    D3D_FEATURE_LEVEL       featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain
        ( nullptr
        , m_driver_type
        , nullptr
        , createDeviceFlags
        , feature_levels
        , _countof(feature_levels)
        , D3D11_SDK_VERSION
        , &sd
        , &m_swap_chain
        , &m_device
        , &featureLevel
        , &m_immediate_contex);

    ID3D11Texture2D* p_back_buffer = NULL;
    hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p_back_buffer);

    hr = m_device->CreateRenderTargetView(p_back_buffer, NULL, &m_render_target_view);
    m_immediate_contex->OMSetRenderTargets(1, &m_render_target_view, 0);
}

void DXDevice::release()
{
    if (m_immediate_contex)
    {
        m_immediate_contex->ClearState();
        m_immediate_contex = nullptr;
    }
    if (m_render_target_view)
    {
        m_render_target_view->Release();
        m_render_target_view = nullptr;
    }
    if (m_swap_chain)
    {
        m_swap_chain->Release();
        m_swap_chain = nullptr;
    }
}

void DXDevice::render()
{
    float ClearColor[4] = { 1.0f, 0.0f, 0.6f, 1.0f };
    m_immediate_contex->ClearRenderTargetView(m_render_target_view, ClearColor);

    m_swap_chain->Present(0, 0);
}
