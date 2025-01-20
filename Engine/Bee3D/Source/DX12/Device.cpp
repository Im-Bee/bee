#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

BEE_DX12_CPP;

// ----------------------------------------------------------------------------
//                              Public Methods 
// ----------------------------------------------------------------------------

b_status Device::Initialize()
{
    BEE_LOG(Debug::Info, L"Device (%p): Initializing", this);

    uint32_t dxgiFactoryFlags = 0;
#ifdef _DEBUG
    ComPtr<ID3D12Debug> debugController(0);
    B_DXGI_HANDLE_FAILURE_BEG(::D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
        BEE_LOG(Debug::Warning, L"RendererDX (%p): Can't get debug interface", this);
    B_DXGI_HANDLE_FAILURE_END;

    debugController->EnableDebugLayer();
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG

    B_DXGI_THROW_IF_FAIL(::CreateDXGIFactory2(
        dxgiFactoryFlags,
        IID_PPV_ARGS(&m_pFactory)));  

    if (BEE_CORRUPTED(CreateItself()))
    {
        BEE_RETURN_BAD;
    }

    BEE_RETURN_SUCCESS;
}

b_status Device::CreateDebugCallback()
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating debug call", this);

#ifdef _DEBUG
    if (m_CallbackCookie)
    {
        BEE_RETURN_OKAY;
    }

    if (!m_pDevice.Get())
    {
        BEE_RETURN_BAD;
    }

    ComPtr<ID3D12InfoQueue1> infoQueue = 0;
    B_DXGI_IF_SUCCEEDED_BEG(m_pDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)));
        void* pContext = nullptr;
        infoQueue->RegisterMessageCallback(Bee::DX12::DirectXLoggingCallback,
                                           ::D3D12_MESSAGE_CALLBACK_IGNORE_FILTERS,
                                           pContext,
                                           &m_CallbackCookie);

        if (!m_CallbackCookie)
        {
            BEE_RETURN_FAIL;
        }
    B_DXGI_IF_SUCCEEDED_END;
#endif // _DEBUG
    
    BEE_RETURN_SUCCESS;
}

b_status Bee::DX12::Device::CreateCommandQueue(SharedPtr<CommandQueue>& pCmd)
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating command queue for %p", this, pCmd.Get());

    ComPtr<ID3D12CommandQueue>     cmdQueue(0);
    ComPtr<ID3D12CommandAllocator> cmdAlloc(0);

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = ::D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type  = ::D3D12_COMMAND_LIST_TYPE_DIRECT;

    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->CreateCommandQueue(&queueDesc,
                                                            IID_PPV_ARGS(&cmdQueue)))
        BEE_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;

    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->CreateCommandAllocator(::D3D12_COMMAND_LIST_TYPE_DIRECT, 
                                                                IID_PPV_ARGS(&cmdAlloc)))
        BEE_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;

    pCmd->m_pCmdQueue = Move(cmdQueue);
    pCmd->m_pCmdAlloc = Move(cmdAlloc);

    BEE_RETURN_SUCCESS;
}

b_status Bee::DX12::Device::CreateSwapChain(SharedPtr<SwapChain>& pSC)
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating swap chain for %p", this, pSC.Get());

    ComPtr<IDXGIFactory2>   factory2(nullptr);
    ComPtr<IDXGISwapChain1> swapChain1(nullptr);

    if (!this->GetRenderer()->GetWindow())
    {
        throw Debug::NullptrCall(BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    const auto& windowWidth  = this->GetRenderer()->GetWindow()->GetProperties().Dimensions.x;
    const auto& windowHeight = this->GetRenderer()->GetWindow()->GetProperties().Dimensions.y;
    const auto& windowHandle = this->GetRenderer()->GetWindow()->GetHandle();

    if (SUCCEEDED(m_pFactory->QueryInterface(IID_PPV_ARGS(&factory2))))
    {
        DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
        swapDesc.BufferCount      = pSC->m_uFrameCount;
        swapDesc.Width            = static_cast<UINT>(windowWidth);
        swapDesc.Height           = static_cast<UINT>(windowHeight);
        swapDesc.Format           = ::DXGI_FORMAT_R8G8B8A8_UNORM;
        swapDesc.BufferUsage      = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapDesc.SwapEffect       = ::DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapDesc.SampleDesc.Count = 1;
        swapDesc.Flags           |= ::DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

        factory2->CreateSwapChainForHwnd(this->GetRenderer()->GetCommandQueue()->m_pCmdQueue.Get(),
                                         windowHandle,
                                         &swapDesc,
                                         nullptr,
                                         nullptr,
                                         &swapChain1);
    }

    if (!swapChain1.Get())
    {
        BEE_RETURN_FAIL;
    }

#ifdef _DEBUG
    static constexpr char szSwapchainName[] = "B_SWAP_CHAIN_";
    swapChain1->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szSwapchainName), szSwapchainName);
#endif // _DEBUG

    pSC->m_pSwapChain = Move(swapChain1);

    BEE_RETURN_SUCCESS;
}

b_status Bee::DX12::Device::CompileShaders(SharedPtr<Resources>& pRsc, 
                                           const wchar_t* szShadersPath)
{
    BEE_LOG(Debug::Info, L"Device (%p): Compiling shaders for %p", this, pRsc.Get());

    if (pRsc->m_pRootSignature.Get())
    {
        BEE_LOG(Debug::Error, L"%p already has a root signature", pRsc.Get());
        BEE_RETURN_BAD;
    }

    if (pRsc->m_pPipelineState.Get())
    {
        BEE_LOG(Debug::Error, L"%p already has a pipeline state", pRsc.Get());
        BEE_RETURN_FAIL;
    }

    ComPtr<ID3D12RootSignature> rootSig(CreateNoSamplersRootSignature(pRsc));
    if (!rootSig.Get())
    {
        BEE_RETURN_BAD;
    }

    pRsc->m_pRootSignature = rootSig;










    BEE_RETURN_SUCCESS;
}

// ----------------------------------------------------------------------------
//                              Private Methods 
// ----------------------------------------------------------------------------

b_status Bee::DX12::Device::CreateItself()
{
    ComPtr<IDXGIFactory6> factory6(nullptr);

    B_DXGI_HANDLE_FAILURE_BEG(m_pFactory->QueryInterface(IID_PPV_ARGS(&factory6)));
        BEE_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;

    for (uint32_t i = 0;
         factory6->EnumAdapterByGpuPreference(i,
                                              ::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                                              IID_PPV_ARGS(&m_pAdapter)) != DXGI_ERROR_NOT_FOUND;
         ++i)
    {
        if (B_WIN_SUCCEEDED(::D3D12CreateDevice(m_pAdapter.Get(),
                                                ::D3D_FEATURE_LEVEL_12_0,
                                                IID_PPV_ARGS(&m_pDevice))))
        {
            BEE_LOG(Debug::Info, L"Device (%p): Succesfully created a device", this);

#ifdef _DEBUG
            static constexpr char szAdapterName[] = "B_ADAPTER_";

            m_pAdapter->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szAdapterName), szAdapterName);
            m_pDevice->SetName(L"B_DEVICE_");
#endif // _DEBUG

            BEE_RETURN_SUCCESS;
        }
    }

    BEE_RETURN_BAD;
}

ComPtr<ID3D12RootSignature> Bee::DX12::Device::CreateNoSamplersRootSignature(SharedPtr<Resources>& pRsc)
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating root signature for %p", this, pRsc.Get());
       
    ComPtr<ID3D12RootSignature> rootSig(0);
    ComPtr<ID3DBlob> signature(0);

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.pParameters = nullptr;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


    B_DXGI_HANDLE_FAILURE_BEG(::D3D12SerializeRootSignature(&rootSignatureDesc,
                                                            D3D_ROOT_SIGNATURE_VERSION_1,
                                                            &signature,
                                                            nullptr));
    B_DXGI_HANDLE_FAILURE_END;

    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->CreateRootSignature(0,
                                                             signature->GetBufferPointer(),
                                                             signature->GetBufferSize(),
                                                             IID_PPV_ARGS(&rootSig)));
    B_DXGI_HANDLE_FAILURE_END;

    return rootSig;
}
