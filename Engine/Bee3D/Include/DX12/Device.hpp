#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Device : public IRendererComponent
    {
        BEE_USING_BEE_DX12;

        friend MemoryManager;

    public:
        Device() = default;
        ~Device() = default;

        Device(Device&&) = default;

// Public Methods -------------------------------------------------------------
    public:
        Status Initialize();

        Status CreateDebugCallback();

        Status CreateCommandQueue(SharedPtr<CommandQueue>&);

        Status CreateSwapChain(SharedPtr<SwapChain>&);

        Status CompileShaders(SharedPtr<MeshResources>& pResources, const wchar_t* wszShadersPath);

// Private Methods ------------------------------------------------------------
    private:
        Status CreateItself();

        ComPtr<ID3D12RootSignature> CreateNoSamplersRootSignature(SharedPtr<MeshResources>&);

        ComPtr<ID3D12PipelineState> CreateVertexPixelPipelineState(D3D12_SHADER_BYTECODE& VS, D3D12_SHADER_BYTECODE& PS, SharedPtr<MeshResources>&);

    private:
        ComPtr<ID3D12Device> m_pDevice  = 0;
        ComPtr<IDXGIAdapter> m_pAdapter = 0;
        ComPtr<IDXGIFactory> m_pFactory = 0;

        DWORD m_CallbackCookie = 0;
    };
#pragma warning(pop)
}
