#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

b_status Bee::DX12::CommandQueue::UpdateFenceValue(
    const ComPtr<ID3D12Fence>& pFence,
    uint64_t uValue) const
{
    if (!pFence.Get())
        BEE_RETURN_BAD;

    if (BEE_WIN_FAILED(m_pCmdQueue->Signal(pFence.Get(), uValue)))
    {
        BEE_RETURN_FAIL;
    }

    BEE_RETURN_SUCCESS;
}