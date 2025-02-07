#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API CommandQueue : public IRendererComponent
    {
        BEE_USING_BEE_DX12;

        friend class Device;

    public:
        CommandQueue() = default;
        ~CommandQueue() = default;

        CommandQueue(CommandQueue&&) = default;
        CommandQueue(const CommandQueue&) = delete;

// Public Methods -------------------------------------------------------------
    public:
        Status OpenQueue(SharedPtr<MeshResources> pResources);

        Status DrawVertices(SharedPtr<MeshResources> pResources);

        Status Execute();

        /**
        * Updates a fence to a specified value from the GPU side
        **/
        Status UpdateFenceValue(const ComPtr<ID3D12Fence>& pFence, uint64_t uValue) const;

    private:
        ComPtr<ID3D12CommandQueue>          m_pCmdQueue = 0;
        ComPtr<ID3D12CommandAllocator>      m_pCmdAlloc = 0;
        ComPtr<ID3D12GraphicsCommandList>   m_pCmdList  = 0;
    };
#pragma warning(pop)
}