#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

BEE_DX12_CPP;

// ----------------------------------------------------------------------------
// RendererDX -----------------------------------------------------------------
// Public Methods -------------------------------------------------------------
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

Bee::DX12::RendererDX::RendererDX(const uint32_t& flags, IWindow* pWindow)
    : m_pWindow(pWindow)
{
    ProcessFlags(flags);
}

// ----------------------------------------------------------------------------

b_status RendererDX::Initialize()
{
    BEE_LOG(Debug::Info, L"RendererDX (%p): Initializing", this);

    if (!BEE_IS_ALREADY_DID(m_pWindow->Initialize()))
    {
        return BEE_CORRUPTION;
    }

    if (!BEE_IS_ALREADY_DID(m_pWindow->Show()))
    {
        return BEE_CORRUPTION;
    }

    m_pDevice        = MakeShared<Device>();
    m_pCommandQueue  = MakeShared<CommandQueue>();
    m_pSwapChain     = MakeShared<SwapChain>();
    m_pMemoryMenager = MakeShared<MemoryManager>();

    m_pResources = MakeShared<MeshResources>();

    m_pDevice       ->InitializeComponent(this);
    m_pCommandQueue ->InitializeComponent(this);
    m_pSwapChain    ->InitializeComponent(this);
    m_pMemoryMenager->InitializeComponent(this);
    
    
    m_pMemoryMenager->BindDevice(m_pDevice);


    if (!BEE_IS_SUCCESS(LoadPipeline()))
    {
        return BEE_CORRUPTION;
    }

    if (!BEE_IS_SUCCESS(LoadAssets()))
    {
        return BEE_CORRUPTION;
    }

    return BEE_SUCCESS;
}

// ----------------------------------------------------------------------------

void RendererDX::Update()
{
}

// ----------------------------------------------------------------------------

void RendererDX::Render()
{
    m_pCommandQueue->OpenQueue(m_pResources);
    m_pCommandQueue->Execute();
    m_pSwapChain->WaitForPreviousFrame();
}

// ----------------------------------------------------------------------------

b_status RendererDX::Destroy()
{
    BEE_LOG(Debug::Info, L"RendererDX (%p): Destroying", this);

    if (m_pResources.Get())
    {
        this->m_pResources.~SharedPtr();
    }

    if (m_pMemoryMenager.Get())
    {
        this->m_pMemoryMenager.~SharedPtr();
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

    if (BEE_WIN_IS_SUCCESS(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
    {
        if (BEE_WIN_IS_FAIL(dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL)))
        {
            return BEE_CORRUPTION;
        }
    }
    else
    {
        return BEE_CORRUPTION;
    }
#endif // _DEBUG

    return BEE_SUCCESS;
}

void Bee::DX12::RendererDX::HandleObjects()
{
    this->Destroy();
}

// ----------------------------------------------------------------------------
// RendererDX -----------------------------------------------------------------
// Private Methods ------------------------------------------------------------
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

void Bee::DX12::RendererDX::SetWindow(IWindow* w)
{
    if (m_pWindow)
    {
        BEE_LOG(Debug::Warning, L"RendererDX (%p): SetWindow, window is already set", this);
    }

    m_pWindow = w;
}

// ----------------------------------------------------------------------------

void Bee::DX12::RendererDX::ProcessFlags(const uint32_t& flags)
{
    if (flags & DX12_RENDERER_MAKE_WINDOW_FLAG)
    {
        m_pWindow = new App::Primitives::EmptyWindow();
    }
}

// ----------------------------------------------------------------------------

b_status Bee::DX12::RendererDX::LoadPipeline()
{
    if (!BEE_IS_SUCCESS(m_pDevice->Initialize()))
    {
        return BEE_CORRUPTION;
    }

    if (!BEE_IS_SUCCESS(m_pDevice->CreateCommandQueue(m_pCommandQueue)))
    {
        return BEE_CORRUPTION;
    }

    if (!BEE_IS_SUCCESS(m_pDevice->CreateSwapChain(m_pSwapChain)))
    {
        return BEE_CORRUPTION;
    }

#ifdef _DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug = 0;

    B_DXGI_HANDLE_FAILURE_BEG(::DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)));
        return BEE_CORRUPTION;
    B_DXGI_HANDLE_FAILURE_END;

    dxgiDebug->EnableLeakTrackingForThread();

    if (BEE_IS_CORRUPTED(m_pDevice->CreateDebugCallback()))
    {
        return BEE_CORRUPTION;
    }
#endif // _DEBUG

    return BEE_SUCCESS;
}

// ----------------------------------------------------------------------------

b_status Bee::DX12::RendererDX::LoadAssets()
{
    bool succsecs = true;

    wchar_t wszFilePath[BEE_MAX_PATH] = { 0 };

    wcscpy_s(wszFilePath, ::Bee::App::Properties::Get().GetResourcesPath());
    wcscat_s(wszFilePath, L"Shaders\\Basic.hlsl");
    if (BEE_IS_COULDNT_DO(m_pDevice->CompileShaders(m_pResources, wszFilePath)))
    {
        B_REPORT_FAILURE();

        succsecs = false;
    }

    wcscpy_s(wszFilePath, ::Bee::App::Properties::Get().GetResourcesPath());
    wcscat_s(wszFilePath, L"Models\\Duck.obj");
    if (BEE_IS_COULDNT_DO(m_pResources->LoadMeshOnCPU(wszFilePath)))
    {
        B_REPORT_FAILURE();

        succsecs = false;
    }
    
    if (succsecs)
    {
        m_pMemoryMenager->AllocateVerticesBufferOnGPU(m_pResources.Get());
    }



    m_pSwapChain->WaitForPreviousFrame();

    return BEE_SUCCESS;
}
