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
    ComPtr<ID3D12Debug> debugController(nullptr);
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

b_status Device::CreateCommandQueue(SharedPtr<CommandQueue>& pCmd)
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating command queue for %p", this, pCmd.Get());

    ComPtr<ID3D12CommandQueue>        cmdQueue(nullptr);
    ComPtr<ID3D12CommandAllocator>    cmdAlloc(nullptr);
    ComPtr<ID3D12GraphicsCommandList> cmdList(nullptr);
    ComPtr<ID3D12Device4>             device4(nullptr);

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

    if (B_WIN_SUCCEEDED(m_pDevice->QueryInterface(IID_PPV_ARGS(&device4))))
    {
        B_DXGI_HANDLE_FAILURE_BEG(device4->CreateCommandList1(0,
                                                              D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                              D3D12_COMMAND_LIST_FLAG_NONE,
                                                              IID_PPV_ARGS(&cmdList)));
        B_DXGI_HANDLE_FAILURE_END;
    }

    if (!cmdQueue.Get())
    {
        BEE_RETURN_FAIL;
    }
    if (!cmdAlloc.Get()) 
    {
        BEE_RETURN_FAIL;
    }
    if (!cmdList.Get())
    {
        BEE_RETURN_FAIL;
    }

    pCmd->m_pCmdQueue = Move(cmdQueue);
    pCmd->m_pCmdAlloc = Move(cmdAlloc);
    pCmd->m_pCmdList  = Move(cmdList);

    BEE_RETURN_SUCCESS;
}

b_status Device::CreateSwapChain(SharedPtr<SwapChain>& pSC)
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating swap chain for %p", this, pSC.Get());

    ComPtr<IDXGIFactory2>   factory2(nullptr);
    ComPtr<IDXGISwapChain1> swapChain1(nullptr);
    ComPtr<ID3D12Fence> fence(nullptr);
    HANDLE fenceEvent;

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

        B_DXGI_HANDLE_FAILURE_BEG(factory2->CreateSwapChainForHwnd(this->GetRenderer()->GetCommandQueue()->m_pCmdQueue.Get(),
                                                                   windowHandle,
                                                                   &swapDesc,
                                                                   nullptr,
                                                                   nullptr,
                                                                   &swapChain1));
        B_DXGI_HANDLE_FAILURE_END;
    }

    if (!swapChain1.Get())
    {
        BEE_RETURN_FAIL;
    }

#ifdef _DEBUG
    static constexpr char szSwapchainName[] = "B_SWAP_CHAIN_";
    if (swapChain1.Get())
    {
        swapChain1->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szSwapchainName), szSwapchainName);
    }
#endif // _DEBUG

    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->CreateFence(0,
                                                     D3D12_FENCE_FLAG_NONE,
                                                     IID_PPV_ARGS(&fence)));
    B_DXGI_HANDLE_FAILURE_END;


    if (!fence.Get())
    {
        BEE_RETURN_FAIL;
    }

#ifdef _DEBUG
    static constexpr char szFenceName[] = "B_FENCE_";
    if (fence.Get())
    {
        fence->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szFenceName), szFenceName);
    }
#endif // _DEBUG

    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent == nullptr)
    {
        B_WIN_REPORT_FAILURE();
        BEE_RETURN_FAIL;
    }

    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
    rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvDesc.NumDescriptors = pSC->m_uFrameCount;
    rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    m_pDevice->CreateDescriptorHeap(&rtvDesc,
                                    IID_PPV_ARGS(&pSC->m_pRtvDescriptorHeap));

    pSC->m_uRtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle(pSC->m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    for (uint32_t i = 0; i < pSC->m_uFrameCount; ++i)
    {
        B_DXGI_HANDLE_FAILURE_BEG(swapChain1->GetBuffer(i,
                                                        IID_PPV_ARGS(&pSC->m_pRenderTargets[i])));
        B_DXGI_HANDLE_FAILURE_END;

        m_pDevice->CreateRenderTargetView(pSC->m_pRenderTargets[i].Get(), nullptr, cpuDescHandle);
        cpuDescHandle.ptr = static_cast<size_t>(cpuDescHandle.ptr) + (static_cast<int64_t>(1) * pSC->m_uRtvDescriptorSize);
    }

    pSC->m_pSwapChain = Move(swapChain1);
    pSC->m_pFence     = Move(fence);
    pSC->m_FenceEvent = Move(fenceEvent);

    BEE_RETURN_SUCCESS;
}

b_status Device::CompileShaders(SharedPtr<MeshResources>& pRsc, 
                                           const wchar_t* wszShadersPath)
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

#ifdef _DEBUG
    uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    uint32_t compileFlags = 0;
#endif // DEBUG

    ComPtr<ID3DBlob> vertexShader(nullptr);
    ComPtr<ID3DBlob> pixelShader(nullptr);

    B_DXGI_HANDLE_FAILURE_BEG(D3DCompileFromFile(wszShadersPath,
                                                 nullptr,
                                                 nullptr,
                                                 "VSMain",
                                                 "vs_5_0",
                                                 compileFlags,
                                                 0,
                                                 &vertexShader,
                                                 nullptr));
        BEE_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;


    B_DXGI_HANDLE_FAILURE_BEG(D3DCompileFromFile(wszShadersPath,
                                                 nullptr,
                                                 nullptr,
                                                 "PSMain",
                                                 "ps_5_0",
                                                 compileFlags,
                                                 0,
                                                 &pixelShader,
                                                 nullptr));
        BEE_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;
            
    D3D12_SHADER_BYTECODE vs(vertexShader.Get()->GetBufferPointer(), vertexShader.Get()->GetBufferSize());
    D3D12_SHADER_BYTECODE ps(pixelShader.Get()->GetBufferPointer(), pixelShader.Get()->GetBufferSize());
    ComPtr<ID3D12PipelineState> pipelineState(CreateVertexPixelPipelineState(vs, ps, pRsc));
    if (!pipelineState.Get())
    {
        BEE_RETURN_BAD;
    }
    pRsc->m_pPipelineState = pipelineState;

    BEE_RETURN_SUCCESS;
}

// ----------------------------------------------------------------------------
//                              Private Methods 
// ----------------------------------------------------------------------------

b_status Device::CreateItself()
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

ComPtr<ID3D12RootSignature> Device::CreateNoSamplersRootSignature(SharedPtr<MeshResources>& pRsc)
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating root signature for %p", this, pRsc.Get());
       
    ComPtr<ID3D12RootSignature> rootSig(nullptr);
    ComPtr<ID3DBlob> signature(nullptr);

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

#ifdef _DEBUG
    static constexpr char szRootSig[] = "B_ROOTSIG_";
    if (rootSig.Get())
    {
        rootSig->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szRootSig), szRootSig);
    }
#endif // _DEBUG

    return rootSig;
}
                                                                                              
ComPtr<ID3D12PipelineState> Device::CreateVertexPixelPipelineState(D3D12_SHADER_BYTECODE&     VS,
                                                                   D3D12_SHADER_BYTECODE&     PS,
                                                                   SharedPtr<MeshResources>&  pRsc)
{
    BEE_LOG(Debug::Info, L"Device (%p): Creating pipeline state for %p", this, pRsc.Get());

    ComPtr<ID3D12PipelineState> pipelineState(nullptr);

    D3D12_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    rasterizerDesc.ForcedSampleCount = 0;
    rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    D3D12_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
    {
        FALSE,FALSE,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
    {
        blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
    desc.InputLayout = { ColorVertex::VertexElementsDesc, _countof(ColorVertex::VertexElementsDesc) };
    desc.pRootSignature = pRsc->m_pRootSignature.Get();
    desc.VS = VS;
    desc.PS = PS;
    desc.RasterizerState = rasterizerDesc;
    desc.BlendState = blendDesc;
    desc.DepthStencilState.DepthEnable = FALSE;
    desc.DepthStencilState.StencilEnable = FALSE;
    desc.SampleMask = UINT_MAX;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;

    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState)));
    B_DXGI_HANDLE_FAILURE_END;

#ifdef _DEBUG
    static constexpr char szPipelineState[] = "B_PIPELINE_";
    if (pipelineState.Get())
    {
        pipelineState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(szPipelineState), szPipelineState);
    }
#endif // _DEBUG

    return pipelineState;
}
