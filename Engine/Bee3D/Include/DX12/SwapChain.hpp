#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
#pragma warning(push)
    // Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API SwapChain : public IRendererComponent
    {
        BEE_USING_BEE_DX12;

        friend class Device;

    public:
        SwapChain() = default;
        ~SwapChain() = default;

        SwapChain(SwapChain&&) = default;

// Public Methods -------------------------------------------------------------
    public:

        D3D12_RESOURCE_BARRIER GetCurrentRenderBarrier();

        D3D12_RESOURCE_BARRIER GetCurrentPresentBarrier();

        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentCpuDescHandle();

        void Present();

        /**
        * Forces itself to wait for the previous frame
        * for infinite amount of time
        **/
        Status WaitForPreviousFrame();

    private:
        static const uint32_t m_uFrameCount = 2;

        ComPtr<IDXGISwapChain> m_pSwapChain = 0;

        ComPtr<ID3D12DescriptorHeap> m_pRtvDescriptorHeap            = 0;
        uint32_t                     m_uRtvDescriptorSize            = 0;
        ComPtr<ID3D12Resource>       m_pRenderTargets[m_uFrameCount] = { 0, 0 };

        ComPtr<ID3D12Fence> m_pFence      = 0;
        HANDLE              m_FenceEvent  = INVALID_HANDLE_VALUE;
        uint64_t            m_uFrameIndex = -1;
        uint64_t            m_uFenceValue = -1;
    };
#pragma warning(pop)
}
