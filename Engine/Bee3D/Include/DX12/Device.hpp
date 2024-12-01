#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
    class CommandQueue;
    class SwapChain;
    class Resources;

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Device : public IRendererComponent
    {
        BEE_USING_BEE_DX12;

    public:
        Device() = default;
        ~Device() = default;

        Device(Device&&) = default;

    public:
// Public Methods -------------------------------------------------------------
        Status Initialize();

        Status CreateDebugCallback();

        Status CreateCommandQueue(SharedPtr<CommandQueue>&);

        Status CreateSwapChain(SharedPtr<SwapChain>&);

        Status CompileShaders(SharedPtr<Resources>& pResources, const wchar_t* szShadersPath);

    private:
// Private Methods ------------------------------------------------------------
        Status CreateItself();

        ComPtr<ID3D12RootSignature> CreateNoSamplersRootSignature(SharedPtr<Resources>&);

    private:
        ComPtr<ID3D12Device> m_pDevice  = 0;
        ComPtr<IDXGIAdapter> m_pAdapter = 0;
        ComPtr<IDXGIFactory> m_pFactory = 0;

        DWORD m_CallbackCookie = 0;
    };
#pragma warning(pop)
}
