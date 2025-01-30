#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

BEE_DX12_CPP;

D3D12_RESOURCE_BARRIER Bee::DX12::SwapChain::GetCurrentRenderBarrier()
{
    D3D12_RESOURCE_BARRIER r = {};
    r.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    r.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    r.Transition.pResource = m_pRenderTargets[m_uFrameIndex].Get();
    r.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    r.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

    return r;
}

D3D12_RESOURCE_BARRIER Bee::DX12::SwapChain::GetCurrentPresentBarrier()
{
    D3D12_RESOURCE_BARRIER r = {};
    r.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    r.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    r.Transition.pResource = m_pRenderTargets[m_uFrameIndex].Get();
    r.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    r.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

    return r;
}

D3D12_CPU_DESCRIPTOR_HANDLE Bee::DX12::SwapChain::GetCurrentCpuDescHandle()
{
    D3D12_CPU_DESCRIPTOR_HANDLE r = {};
    r.ptr = static_cast<size_t>(m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr) + (static_cast<int64_t>(m_uFrameIndex) * m_uRtvDescriptorSize);
    return r;
}

void SwapChain::Present()
{
    m_pSwapChain->Present(1, 0);
}

b_status Bee::DX12::SwapChain::WaitForPreviousFrame()
{
    ComPtr<IDXGISwapChain3> swapChain3(nullptr);

    const uint64_t fence = m_uFenceValue++;

    BEE_THROW_IF_FAILED(this->GetRenderer()->GetCommandQueue()->UpdateFenceValue(m_pFence, fence));

    // Wait for previous frame
    if (m_pFence->GetCompletedValue() < fence)
    {
        B_DXGI_THROW_IF_FAIL(m_pFence->SetEventOnCompletion(fence, m_FenceEvent));
        WaitForSingleObject(m_FenceEvent, INFINITE);
    }

    if (B_WIN_SUCCEEDED(m_pSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain3))))
    {
        m_uFrameIndex = swapChain3->GetCurrentBackBufferIndex();
        BEE_RETURN_SUCCESS;
    }

    BEE_RETURN_BAD;
}
