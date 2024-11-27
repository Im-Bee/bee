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
    BEE_LOG(Problems::Info, L"DirectX: %S", pDescription);
}

// ----------------------------------------------------------------------------
//                                   Renderer
// 
//                                Public Methods
// ----------------------------------------------------------------------------

Bee::DX12::Renderer::Renderer(const uint32_t& flags)
{
    ProcessFlags(flags);
}

Bee::DX12::Renderer::Renderer(IWindow* wnd, const uint32_t& flags)
    : m_pWindow(wnd)
{
    ProcessFlags(flags);
}

b_status Renderer::Initialize()
{
    BEE_LOG(Problems::Info, L"Renderer (%p): Initializing", this);

    if (!B_IS_OKAY(m_pWindow->Initialize()))
        B_RETURN_FAIL;

    if (!B_IS_OKAY(m_pWindow->Show()))
        B_RETURN_FAIL;

    m_pDevice       = MakeShared<Device>();
    m_pCommandQueue = MakeShared<CommandQueue>();
    m_pSwapChain    = MakeShared<SwapChain>();

    m_pDevice->InitializeComponent(this);
    m_pCommandQueue->InitializeComponent(this);
    m_pSwapChain->InitializeComponent(this);

    if (!B_IS_OKAY(LoadPipeline()))
        B_RETURN_BAD;

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
    BEE_LOG(Problems::Info, L"Renderer (%p): Destroying", this);

    if (m_pSwapChain)
        this->m_pSwapChain.~SharedPtr();
    if (m_pCommandQueue)
        this->m_pCommandQueue.~SharedPtr();
    if (m_pDevice)
        this->m_pDevice.~SharedPtr(); 

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;
    if (B_WIN_SUCCEEDED(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
    {
        if (B_WIN_FAILED(dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL)))
            B_RETURN_FAIL;
    }
    else
    {
        B_RETURN_FAIL;
    }
#endif // _DEBUG

    B_RETURN_SUCCESS;
}

void Bee::DX12::Renderer::Dump()
{
    this->Destroy();
}

// ----------------------------------------------------------------------------
//                                   Renderer
// 
//                                Private Methods
// ----------------------------------------------------------------------------

void Bee::DX12::Renderer::SetWindow(IWindow* w)
{
    if (m_pWindow)
    {
        BEE_LOG(Problems::Warning, L"Renderer (%p): SetWindow, window is already set", this);
    }

    m_pWindow = w;
}

void Bee::DX12::Renderer::ProcessFlags(const uint32_t& flags)
{
    if (flags & DX12_RENDERER_MAKE_WINDOW_FLAG)
        m_pWindow = new App::Primitives::EmptyWindow();
}

b_status Bee::DX12::Renderer::LoadPipeline()
{
    if (!B_IS_SUCCESS(m_pDevice->Initialize()))
        B_RETURN_BAD;

    if (!B_IS_SUCCESS(m_pDevice->CreateCommandQueue(m_pCommandQueue)))
        B_RETURN_BAD;
    if (!B_IS_SUCCESS(m_pDevice->CreateSwapChain(m_pSwapChain)))
        B_RETURN_BAD;

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;
    B_DXGI_HANDLE_FAILURE_BEG(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
        B_RETURN_OKAY;
    B_DXGI_HANDLE_FAILURE_END;

    dxgiDebug->EnableLeakTrackingForThread();

    if (B_IS_BAD(m_pDevice->CreateDebugCallback()))
        B_RETURN_BAD;
#endif // _DEBUG

    B_RETURN_SUCCESS;
}
