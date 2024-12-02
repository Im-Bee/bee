#include "Bee3D.hpp"

BEE_DX12_CPP;

// ----------------------------------------------------------------------------
//                                   RendererDX
// 
//                                Public Methods
// ----------------------------------------------------------------------------

Bee::DX12::RendererDX::RendererDX(const uint32_t& flags)
{
    ProcessFlags(flags);
}

Bee::DX12::RendererDX::RendererDX(IWindow* wnd, const uint32_t& flags)
    : m_pWindow(wnd)
{
    ProcessFlags(flags);
}

b_status RendererDX::Initialize()
{
    BEE_LOG(Problems::Info, L"RendererDX (%p): Initializing", this);

    if (!BEE_WORKED(m_pWindow->Initialize()))
        BEE_RETURN_FAIL;

    if (!BEE_WORKED(m_pWindow->Show()))
        BEE_RETURN_FAIL;

    m_pDevice       = MakeShared<Device>();
    m_pCommandQueue = MakeShared<CommandQueue>();
    m_pSwapChain    = MakeShared<SwapChain>();
    m_pResources    = MakeShared<Resources>();

    m_pDevice->InitializeComponent(this);
    m_pCommandQueue->InitializeComponent(this);
    m_pSwapChain->InitializeComponent(this);
    m_pResources->InitializeComponent(this);

    if (!BEE_WORKED(LoadPipeline()))
        BEE_RETURN_BAD;

    if (!BEE_WORKED(LoadAssets()))
        BEE_RETURN_BAD;

    BEE_RETURN_SUCCESS;
}

void RendererDX::Update()
{
}

void RendererDX::Render()
{
}

b_status RendererDX::Destroy()
{
    BEE_LOG(Problems::Info, L"RendererDX (%p): Destroying", this);

    if (m_pResources.Get())
        this->m_pResources.~SharedPtr();
    if (m_pSwapChain.Get())
        this->m_pSwapChain.~SharedPtr();
    if (m_pCommandQueue.Get())
        this->m_pCommandQueue.~SharedPtr();
    if (m_pDevice.Get())
        this->m_pDevice.~SharedPtr(); 

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;
    if (BEE_WIN_SUCCEEDED(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
    {
        if (BEE_WIN_FAILED(dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL)))
            BEE_RETURN_FAIL;
    }
    else
    {
        BEE_RETURN_FAIL;
    }
#endif // _DEBUG

    BEE_RETURN_SUCCESS;
}

void Bee::DX12::RendererDX::Dump()
{
    this->Destroy();
}

// ----------------------------------------------------------------------------
//                                   RendererDX
// 
//                                Private Methods
// ----------------------------------------------------------------------------

void Bee::DX12::RendererDX::SetWindow(IWindow* w)
{
    if (m_pWindow)
    {
        BEE_LOG(Problems::Warning, L"RendererDX (%p): SetWindow, window is already set", this);
    }

    m_pWindow = w;
}

void Bee::DX12::RendererDX::ProcessFlags(const uint32_t& flags)
{
    if (flags & DX12_RENDERER_MAKE_WINDOW_FLAG)
        m_pWindow = new App::Primitives::EmptyWindow();
}

b_status Bee::DX12::RendererDX::LoadPipeline()
{
    if (!BEE_SUCCEEDED(m_pDevice->Initialize()))
        BEE_RETURN_BAD;

    if (!BEE_SUCCEEDED(m_pDevice->CreateCommandQueue(m_pCommandQueue)))
        BEE_RETURN_BAD;
    if (!BEE_SUCCEEDED(m_pDevice->CreateSwapChain(m_pSwapChain)))
        BEE_RETURN_BAD;

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;
    B_DXGI_HANDLE_FAILURE_BEG(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
        BEE_RETURN_OKAY;
    B_DXGI_HANDLE_FAILURE_END;

    dxgiDebug->EnableLeakTrackingForThread();

    if (BEE_CORRUPTED(m_pDevice->CreateDebugCallback()))
        BEE_RETURN_BAD;
#endif // _DEBUG

    BEE_RETURN_SUCCESS;
}

b_status Bee::DX12::RendererDX::LoadAssets()
{
    wchar_t szShadersPath[B_MAX_PATH] = { 0 };
    wcscpy_s(szShadersPath, Bee::App::Properties::Get().GetResourcesPath());
    wcscat_s(szShadersPath, L"\\Shaders\\");

    m_pDevice->CompileShaders(m_pResources, szShadersPath);

    // m_pSwapChain->WaitForPreviousFrame();

    BEE_RETURN_SUCCESS;
}
