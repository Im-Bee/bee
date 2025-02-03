#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

BEE_DX12_CPP;

b_status CommandQueue::OpenQueue(SharedPtr<MeshResources> pResources)
{
    B_DXGI_HANDLE_FAILURE_BEG(m_pCmdAlloc->Reset());
        return BEE_CORRUPTION;
    B_DXGI_HANDLE_FAILURE_END;

    B_DXGI_HANDLE_FAILURE_BEG(m_pCmdList->Reset(m_pCmdAlloc.Get(), pResources->m_pPipelineState.Get()));
        return BEE_CORRUPTION;
    B_DXGI_HANDLE_FAILURE_END;

    auto windowDim(this->GetRenderer()->GetWindow()->GetCurrentDimensions());
    D3D12_VIEWPORT viewPort(0, 0, windowDim.x, windowDim.y, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH);
    D3D12_RECT scissorView(0, 0, static_cast<LONG>(windowDim.x), static_cast<LONG>(windowDim.y));

    m_pCmdList->SetGraphicsRootSignature(pResources->m_pRootSignature.Get());
    m_pCmdList->RSSetViewports(1, &viewPort);
    m_pCmdList->RSSetScissorRects(1, &scissorView);

    auto barrier(this->GetRenderer()->GetSwapChain()->GetCurrentRenderBarrier());
    m_pCmdList->ResourceBarrier(1, &barrier);

    auto cpuDescHandle(this->GetRenderer()->GetSwapChain()->GetCurrentCpuDescHandle());
    m_pCmdList->OMSetRenderTargets(1, &cpuDescHandle, FALSE, nullptr);

    constexpr static const float clearColor[] = { 0.9f, 0.7f, 0.08f, 1.0f };
    m_pCmdList->ClearRenderTargetView(cpuDescHandle, clearColor, 0, nullptr);
    m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return BEE_SUCCESS;
}

b_status CommandQueue::DrawVertices(SharedPtr<MeshResources> pResources)
{
    m_pCmdList->IASetVertexBuffers(0, 1, &pResources->GetGPUTrianglesView());
    m_pCmdList->DrawInstanced(pResources->GetSize() * 3, 1, 0, 0);

    return BEE_SUCCESS;
}

b_status CommandQueue::Execute()
{
    auto barrier(this->GetRenderer()->GetSwapChain()->GetCurrentPresentBarrier());
    m_pCmdList->ResourceBarrier(1, &barrier);

    m_pCmdList->Close();

    ID3D12CommandList* pCommandLists[] = { m_pCmdList.Get() };
    m_pCmdQueue->ExecuteCommandLists(_countof(pCommandLists), pCommandLists);

    this->GetRenderer()->GetSwapChain()->Present();

    return BEE_SUCCESS;
}

b_status CommandQueue::UpdateFenceValue(const ComPtr<ID3D12Fence>& pFence,
                                                         uint64_t  uValue) const
{
    if (!pFence.Get())
    {
        return BEE_CORRUPTION;
    }

    if (BEE_WIN_IS_FAIL(m_pCmdQueue->Signal(pFence.Get(), uValue)))
    {
        return BEE_CORRUPTION;
    }

    return BEE_SUCCESS;
}
