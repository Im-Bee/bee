#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API CommandQueue : public IComponent
    {
        BEE_USING_BEE_DX12;

        friend Device;

    public:
        CommandQueue() = default;
        ~CommandQueue() = default;

        CommandQueue(CommandQueue&&) = default;

// Private Methods ------------------------------------------------------------
    private:
        ComPtr<ID3D12CommandQueue>     m_pCmdQueue = 0;
        ComPtr<ID3D12CommandAllocator> m_pCmdAlloc = 0;
    };
#pragma warning(pop)
}