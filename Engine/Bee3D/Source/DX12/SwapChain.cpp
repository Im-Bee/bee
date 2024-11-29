#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

b_status Bee::DX12::SwapChain::WaitForPreviousFrame()
{
    ComPtr<IDXGISwapChain3> swapChain3(nullptr);

    const uint64_t fence = m_uFenceValue++;

    BEE_THROW_IF_FAIL(this->GetRenderer()->GetCommandQueue()->UpdateFenceValue(m_pFence, fence));

    // Wait for previous frame
    if (m_pFence->GetCompletedValue() < fence)
    {
        BEE_DXGI_THROW_IF_FAIL(m_pFence->SetEventOnCompletion(fence, m_FenceEvent));
        WaitForSingleObject(m_FenceEvent, INFINITE);
    }

    if (BEE_WIN_SUCCEEDED(m_pSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain3))))
    {
        m_uFrameIndex = swapChain3->GetCurrentBackBufferIndex();
        BEE_RETURN_SUCCESS;
    }

    BEE_RETURN_BAD;
}
