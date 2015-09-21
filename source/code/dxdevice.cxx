#include "dxdevice.hxx"
#include "app.hxx"
#include <fstream>

DXDevice::DXDevice()
: m_device(NULL)
, m_device_context(NULL)
, m_render_target_view(NULL)
, m_swap_chain(NULL)
{}
DXDevice::~DXDevice(){this->release();}

void DXDevice::initialize(HWND hwnd, int w, int h, bool is_fullscreen)
{
    m_display_size.x = w;
    m_display_size.y = h;

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
        , &m_device_context);
    assert(hr == S_OK);

    ID3D11Texture2D* p_back_buffer = NULL;
    hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p_back_buffer);
    assert(hr == S_OK);
    hr = m_device->CreateRenderTargetView(p_back_buffer, NULL, &m_render_target_view);
    assert(hr == S_OK);
    create_depth_stencil_buffer();

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (FLOAT)w;
    viewport.Height = (FLOAT)h;

    m_device_context->RSSetViewports(1, &viewport);
    m_device_context->OMSetDepthStencilState(m_depth_stencil_state, 1);
    m_device_context->OMSetRenderTargets(1, &m_render_target_view, m_depth_stencil_view);

    create_pixel_shader();
    create_vertex_shader();

    create_vertex_buffer();
    create_index_buffer();
    create_matrices();
    create_constant_buffer();
    create_input_layout();

    m_device_context->PSSetShader(m_pixel_shader, nullptr, 0);
    m_device_context->VSSetShader(m_vertex_shader, nullptr, 0);

    unsigned int zero = 0;
    unsigned int stride = 12;
    m_device_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &zero);
    m_device_context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
    m_device_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ID3D11RasterizerState * g_pRasterState;

    D3D11_RASTERIZER_DESC rasterizerState;
    rasterizerState.FillMode = D3D11_FILL_SOLID;
    rasterizerState.CullMode = D3D11_CULL_NONE;
    rasterizerState.FrontCounterClockwise = true;
    rasterizerState.DepthBias = false;
    rasterizerState.DepthBiasClamp = 0;
    rasterizerState.SlopeScaledDepthBias = 0;
    rasterizerState.DepthClipEnable = false;
    rasterizerState.ScissorEnable = false;
    rasterizerState.MultisampleEnable = false;
    rasterizerState.AntialiasedLineEnable = false;
    hr = m_device->CreateRasterizerState(&rasterizerState, &g_pRasterState);
    assert(hr == S_OK);
    m_device_context->RSSetState(g_pRasterState);
}

void DXDevice::release()
{
    if (m_device_context)
    {
        m_device_context->ClearState();
        m_device_context = nullptr;
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


void DXDevice::update()
{
    

}

void DXDevice::render()
{
    float ClearColor[4] = { 1.0f, 0.0f, 0.3f, 1.0f };
    m_device_context->ClearRenderTargetView(m_render_target_view, ClearColor);
    // begin

    m_device_context->DrawIndexed(12*3, 0, 0);

    // end
    m_swap_chain->Present(0, 0);
}


void DXDevice::load_shader_content()
{
    if (m_vertex_shader_content)
        return;

    std::ifstream vs_fin("simple_vs.cso", std::ios::binary);
    if (vs_fin.is_open())
    {
        vs_fin.seekg(0, std::ios_base::end);
        m_vertex_shader_size = (size_t)vs_fin.tellg();
        m_vertex_shader_content = new char[m_vertex_shader_size];
        vs_fin.seekg(0, std::ios_base::beg);
        vs_fin.read(m_vertex_shader_content, m_vertex_shader_size);
        vs_fin.close();
    }

    if (m_pixel_shader_content)
        return;

    std::ifstream ps_fin("simple_ps.cso", std::ios::binary);
    if (ps_fin.is_open())
    {
        ps_fin.seekg(0, std::ios_base::end);
        m_pixel_shader_size = (size_t)ps_fin.tellg();
        m_pixel_shader_content = new char[m_pixel_shader_size];
        ps_fin.seekg(0, std::ios_base::beg);
        ps_fin.read(m_pixel_shader_content, m_pixel_shader_size);
        ps_fin.close();
    }
}

void DXDevice::create_pixel_shader()
{
    load_shader_content();
    const HRESULT result = m_device->CreatePixelShader(m_pixel_shader_content, m_pixel_shader_size, nullptr, &m_pixel_shader);
    assert(result == S_OK);
}

void DXDevice::create_vertex_shader()
{
    load_shader_content();
    const HRESULT result = m_device->CreateVertexShader(m_vertex_shader_content, m_vertex_shader_size, nullptr, &m_vertex_shader);
    assert(result == S_OK);
}

void DXDevice::create_matrices()
{
    m_view = DirectX::XMMatrixTranslation(0.0f, 0.0f, -1.0f);
    m_world = DirectX::XMMatrixIdentity();
    m_projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (float)m_display_size.x / m_display_size.y, 1.0f, 1000.0f);
}

void DXDevice::create_constant_buffer()
{
    /*D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(DirectX::XMMATRIX)*3;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem*/
}

void DXDevice::create_vertex_buffer()
{
    DirectX::XMFLOAT3 vertices[8];
    // top
    vertices[0] = DirectX::XMFLOAT3(-0.5f, 1.0f, -0.5f);
    vertices[1] = DirectX::XMFLOAT3(-0.5f, 1.0f, 0.5f);
    vertices[2] = DirectX::XMFLOAT3(0.5f, 1.0f, 0.5f);
    vertices[3] = DirectX::XMFLOAT3(0.5f, 1.0f, -0.5f);

    //Bottom
    vertices[4] = DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f);
    vertices[5] = DirectX::XMFLOAT3(-0.5f, 0.0f, 0.5f);
    vertices[6] = DirectX::XMFLOAT3(0.5f, 0.0f, 0.5f);
    vertices[7] = DirectX::XMFLOAT3(0.5f, 0.0f, -0.5f);

    // vertex buffer
    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
    desc.ByteWidth = sizeof(vertices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &vertices;
    const HRESULT result = m_device->CreateBuffer(&desc, &data, &m_vertex_buffer);
    assert(result == S_OK);

}

void DXDevice::create_index_buffer()
{
    int indices[36];

    // Top
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 0; indices[4] = 2; indices[5] = 3;

    // Bottom
    indices[6] = 4; indices[7] = 5; indices[8] = 6;
    indices[9] = 4; indices[10] = 6; indices[11] = 7;

    // Left
    indices[12] = 4; indices[13] = 5; indices[14] = 1;
    indices[15] = 4; indices[16] = 1; indices[17] = 0;

    // Right
    indices[18] = 6; indices[19] = 7; indices[20] = 3;
    indices[21] = 6; indices[22] = 3; indices[23] = 2;

    // Front
    indices[24] = 4; indices[25] = 0; indices[26] = 3;
    indices[27] = 4; indices[28] = 3; indices[29] = 7;

    // Back
    indices[30] = 6; indices[31] = 2; indices[32] = 1;
    indices[33] = 6; indices[34] = 1; indices[35] = 5;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(indices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &indices;

    const HRESULT result = m_device->CreateBuffer(&desc, &data, &m_index_buffer);
    assert(result == S_OK);
}

void DXDevice::create_input_layout()
{

}

void DXDevice::create_depth_stencil_buffer()
{
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = m_display_size.x;
    descDepth.Height = m_display_size.y;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    const HRESULT result = m_device->CreateTexture2D(&descDepth, NULL, &m_depth_stencil_buffer);

    D3D11_DEPTH_STENCIL_DESC dsDesc;
    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    // Stencil test parameters
    dsDesc.StencilEnable = true;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    m_device->CreateDepthStencilState(&dsDesc, &m_depth_stencil_state);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    HRESULT hr;
    hr = m_device->CreateDepthStencilView
        ( m_depth_stencil_buffer // Depth stencil texture
        , &descDSV // Depth stencil desc
        , &m_depth_stencil_view );
}