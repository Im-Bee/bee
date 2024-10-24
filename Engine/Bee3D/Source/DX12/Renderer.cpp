#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

void Bee::DX12::DirectXLoggingCallback(
    D3D12_MESSAGE_CATEGORY Category, 
    D3D12_MESSAGE_SEVERITY Severity, 
    D3D12_MESSAGE_ID ID, 
    LPCSTR pDescription, 
    void* pContext)
{
    B_LOG(Problems::Info, L"DirectX: %S", pDescription);
}

Bee::DX12::Renderer::Renderer(const uint32_t& flags)
{
    if (flags & BEE_DX12_RENDERER_MAKE_WINDOW_FLAG)
        m_pWindow = new Bee::App::Primitives::EmptyWindow();
}

b_status Renderer::Initialize()
{
    B_LOG(Problems::Info, L"Renderer (%p): Initializing", this);

    uint32_t dxgiFactoryFlags = 0;
#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugController = 0;
    B_DXGI_HANDLE_FAILURE_BEG(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        B_LOG(Problems::Warning, L"Renderer (%p): Can't get debug interface", this);
    B_DXGI_HANDLE_FAILURE_END;

    debugController->EnableDebugLayer();
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG

    if (!B_IS_OKAY(m_pWindow->Initialize()))
        B_RETURN_FAIL;

    if (!B_IS_OKAY(m_pWindow->Show()))
        B_RETURN_FAIL;

    m_pDevice       = MakeShared<Device>(Device());
    m_pCommandQueue = MakeShared<CommandQueue>(CommandQueue());

    m_pDevice->InitializeComponent(this);
    m_pCommandQueue->InitializeComponent(this);

    ComPtr<IDXGIFactory> factory = 0;
    B_DXGI_THROW_IF_FAIL(CreateDXGIFactory2(
        dxgiFactoryFlags, 
        IID_PPV_ARGS(&factory)));

    if (!B_IS_OKAY(m_pDevice->Initialize(factory)))
        B_RETURN_BAD;

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;
    B_DXGI_HANDLE_FAILURE_BEG(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
        B_RETURN_OKAY;
    B_DXGI_HANDLE_FAILURE_END;

    dxgiDebug->EnableLeakTrackingForThread();
    
    if (!B_IS_OKAY(m_pDevice->CreateDebugCallback()))
        B_RETURN_FAIL;
#endif // _DEBUG

    B_RETURN_SUCCESS;
}

void Renderer::Update()
{
}

void Renderer::Render()
{
}

b_status Renderer::Destroy()
{
    B_LOG(Problems::Info, L"Renderer (%p): Destroying", this);

    this->m_pDevice.~SharedPtr(); 

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;
    if (B_WIN_SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
        dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
#endif // _DEBUG

    B_RETURN_SUCCESS;
}
