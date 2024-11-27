#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
    class CommandQueue;
    class SwapChain;

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Device : public IComponent
    {
        BEE_USING_BEE_DX12;

    public:
        Device() = default;
        ~Device() = default;

        Device(Device&&) = default;

// Public Methods -------------------------------------------------------------
    public:
        Status Initialize();

        Status CreateDebugCallback();

        Status CreateCommandQueue(SharedPtr<CommandQueue>);

        Status CreateSwapChain(SharedPtr<SwapChain>);

// Private Methods ------------------------------------------------------------
    private:
        Status CreateItself();

    private:
        ComPtr<ID3D12Device> m_pDevice  = 0;
        ComPtr<IDXGIAdapter> m_pAdapter = 0;
        ComPtr<IDXGIFactory> m_pFactory = 0;

        DWORD m_CallbackCookie = 0;
    };
#pragma warning(pop)
}
