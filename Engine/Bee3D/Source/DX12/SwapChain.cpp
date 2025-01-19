#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

BEE_DX12_CPP;

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
