#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

b_status Device::Initialize()
{
    B_LOG(Problems::Info, L"Device (%p): Initializing", this);

    uint32_t dxgiFactoryFlags = 0;
#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugController = 0;
    B_DXGI_HANDLE_FAILURE_BEG(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
    B_LOG(Problems::Warning, L"Renderer (%p): Can't get debug interface", this);
    B_DXGI_HANDLE_FAILURE_END;

    debugController->EnableDebugLayer();
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG

    B_DXGI_THROW_IF_FAIL(CreateDXGIFactory2(
        dxgiFactoryFlags,
        IID_PPV_ARGS(&m_pFactory)));  

    if (B_IS_BAD(CreateItself()))
        B_RETURN_BAD;

    B_RETURN_SUCCESS;
}

b_status Device::CreateDebugCallback()
{
    B_LOG(Problems::Info, L"Device (%p): Creating debug call", this);

#ifdef _DEBUG
    if (m_CallbackCookie)
        B_RETURN_OKAY;

    if (!m_pDevice.Get())
        B_RETURN_BAD;

    ComPtr<ID3D12InfoQueue1> infoQueue = 0;
    B_DXGI_IF_SUCCEEDED_BEG(m_pDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)));
        void* pContext = nullptr;
        infoQueue->RegisterMessageCallback(
            Bee::DX12::DirectXLoggingCallback,
            D3D12_MESSAGE_CALLBACK_IGNORE_FILTERS,
            pContext,
            &m_CallbackCookie);

        if (!m_CallbackCookie)
            B_RETURN_FAIL;
    B_DXGI_IF_SUCCEEDED_END;
#endif // _DEBUG
    
    B_RETURN_SUCCESS;
}

b_status Bee::DX12::Device::CreateCommandQueue(SharedPtr<CommandQueue> pCmd)
{
    B_LOG(Problems::Info, L"Device (%p): Creating command queue for %p", this, pCmd.GetPtr());

    ComPtr<ID3D12CommandQueue>     cmdQueue;
    ComPtr<ID3D12CommandAllocator> cmdAlloc;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->CreateCommandQueue(
        &queueDesc,
        IID_PPV_ARGS(&cmdQueue)))
        B_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;

    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        IID_PPV_ARGS(&cmdAlloc)))
        B_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;

    pCmd->m_pCmdQueue = Move(cmdQueue);
    pCmd->m_pCmdAlloc = Move(cmdAlloc);

    B_RETURN_SUCCESS;
}

b_status Bee::DX12::Device::CreateSwapChain(SharedPtr<SwapChain> pSC)
{
    B_LOG(Problems::Info, L"Device (%p): Creating swap chain for %p", this, pSC.GetPtr());

    ComPtr<IDXGIFactory2>   factory2   = 0;
    ComPtr<IDXGISwapChain1> swapChain1 = 0;

    if (!this->GetRenderer()->GetWindow())
        throw Problems::CallOnNullptr(B_COLLECT_DATA());

    const auto& windowWidth  = this->GetRenderer()->GetWindow()->GetProperties().Dimensions.x;
    const auto& windowHeight = this->GetRenderer()->GetWindow()->GetProperties().Dimensions.y;
    const auto& windowHandle = this->GetRenderer()->GetWindow()->GetHandle();

    if (SUCCEEDED(m_pFactory->QueryInterface(IID_PPV_ARGS(&factory2))))
    {
        DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
        swapDesc.BufferCount = pSC->m_uFrameCount;
        swapDesc.Width = windowWidth;
        swapDesc.Height = windowHeight;
        swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapDesc.SampleDesc.Count = 1;
        swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        factory2->CreateSwapChainForHwnd(
            this->GetRenderer()->GetCommandQueue()->m_pCmdQueue.Get(),
            windowHandle,
            &swapDesc,
            nullptr,
            nullptr,
            &swapChain1);
    }

    if (!swapChain1.Get())
        B_RETURN_FAIL;

#ifdef _DEBUG
    static const char szSwapchainName[] = "B_SWAP_CHAIN_";
    swapChain1->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szSwapchainName), szSwapchainName);
#endif // _DEBUG

    pSC->m_pSwapChain = swapChain1;

    B_RETURN_SUCCESS;
}

b_status Bee::DX12::Device::CreateItself()
{
    ComPtr<IDXGIFactory6> factory6;

    B_DXGI_HANDLE_FAILURE_BEG(m_pFactory->QueryInterface(IID_PPV_ARGS(&factory6)));
        B_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;

    for (
        uint32_t i = 0;
        factory6->EnumAdapterByGpuPreference(
            i,
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(&m_pAdapter)) != DXGI_ERROR_NOT_FOUND;
            ++i)
    {
        if (B_WIN_SUCCEEDED(D3D12CreateDevice(
            m_pAdapter.Get(),
            D3D_FEATURE_LEVEL_12_0,
            IID_PPV_ARGS(&m_pDevice))))
        {
            B_LOG(Problems::Info, L"Device (%p): Succesfully created a device", this);

#ifdef _DEBUG
            static const char szAdapterName[] = "B_ADAPTER_";

            m_pAdapter->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szAdapterName), szAdapterName);
            m_pDevice->SetName(L"B_DEVICE_");
#endif // _DEBUG

            B_RETURN_SUCCESS;
        }
    }

    B_RETURN_BAD;
}
