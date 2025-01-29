#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

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
    BEE_LOG(Debug::Info, L"RendererDX (%p): Initializing", this);

    if (!BEE_IS_OKAY(m_pWindow->Initialize()))
    {
        BEE_RETURN_FAIL;
    }

    if (!BEE_IS_OKAY(m_pWindow->Show()))
    {
        BEE_RETURN_FAIL;
    }

    m_pDevice       = MakeShared<Device>();
    m_pCommandQueue = MakeShared<CommandQueue>();
    m_pSwapChain    = MakeShared<SwapChain>();
    m_pResources    = MakeShared<MeshResources>();

    m_pDevice      ->InitializeComponent(this);
    m_pCommandQueue->InitializeComponent(this);
    m_pSwapChain   ->InitializeComponent(this);
    m_pResources   ->InitializeComponent(this);

    if (!BEE_IS_OKAY(LoadPipeline()))
    {
        BEE_RETURN_BAD;
    }

    if (!BEE_IS_OKAY(LoadAssets()))
    {
        BEE_RETURN_BAD;
    }

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
    BEE_LOG(Debug::Info, L"RendererDX (%p): Destroying", this);

    if (m_pResources.Get())
    {
        this->m_pResources.~SharedPtr();
    }
    if (m_pSwapChain.Get())
    {
        this->m_pSwapChain.~SharedPtr();
    }
    if (m_pCommandQueue.Get())
    {
        this->m_pCommandQueue.~SharedPtr();
    }
    if (m_pDevice.Get())
    {
        this->m_pDevice.~SharedPtr();
    }

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;

    if (B_WIN_SUCCEEDED(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
    {
        if (B_WIN_FAILED(dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL)))
        {
            BEE_RETURN_FAIL;
        }
    }
    else
    {
        BEE_RETURN_FAIL;
    }
#endif // _DEBUG

    BEE_RETURN_SUCCESS;
}

void Bee::DX12::RendererDX::OnException()
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
        BEE_LOG(Debug::Warning, L"RendererDX (%p): SetWindow, window is already set", this);
    }

    m_pWindow = w;
}

void Bee::DX12::RendererDX::ProcessFlags(const uint32_t& flags)
{
    if (flags & DX12_RENDERER_MAKE_WINDOW_FLAG)
    {
        m_pWindow = new App::Primitives::EmptyWindow();
    }
}

b_status Bee::DX12::RendererDX::LoadPipeline()
{
    if (!BEE_SUCCEEDED(m_pDevice->Initialize()))
    {
        BEE_RETURN_BAD;
    }

    if (!BEE_SUCCEEDED(m_pDevice->CreateCommandQueue(m_pCommandQueue)))
    {
        BEE_RETURN_BAD;
    }

    if (!BEE_SUCCEEDED(m_pDevice->CreateSwapChain(m_pSwapChain)))
    {
        BEE_RETURN_BAD;
    }

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;

    B_DXGI_HANDLE_FAILURE_BEG(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
        BEE_RETURN_OKAY;
    B_DXGI_HANDLE_FAILURE_END;

    dxgiDebug->EnableLeakTrackingForThread();

    if (BEE_CORRUPTED(m_pDevice->CreateDebugCallback()))
    {
        BEE_RETURN_BAD;
    }
#endif // _DEBUG

    BEE_RETURN_SUCCESS;
}

b_status Bee::DX12::RendererDX::LoadAssets()
{
    wchar_t wszFilePath[BEE_MAX_PATH] = { 0 };

    wcscpy_s(wszFilePath, ::Bee::App::Properties::Get().GetResourcesPath());
    wcscat_s(wszFilePath, L"Shaders\\Basic.hlsl");

    if (BEE_FAILED(m_pDevice->CompileShaders(m_pResources, wszFilePath)))
    {
        B_REPORT_FAILURE();
    }

    wcscpy_s(wszFilePath, ::Bee::App::Properties::Get().GetResourcesPath());
    wcscat_s(wszFilePath, L"Models\\Duck.obj");
    if (BEE_FAILED(m_pResources->LoadMesh(wszFilePath)))
    {
        B_REPORT_FAILURE();
    }

    // m_pSwapChain->WaitForPreviousFrame();

    BEE_RETURN_SUCCESS;
}
