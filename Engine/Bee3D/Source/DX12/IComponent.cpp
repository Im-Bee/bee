#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

#include "../Include/DX12/CommandQueue.hpp"
#include "../Include/DX12/Device.hpp"
#include "../Include/DX12/Resources.hpp"
#include "../Include/DX12/SwapChain.hpp"

BEE_DX12_CPP;

b_status Bee::DX12::IRendererComponent::InitializeComponent(RendererDX* p)
{
    if (m_pRenderer)
    {
        BEE_LOG(Debug::Warning, L"IRendererComponent (%p): IRendererComponent was already initialized. Proceeding anyway.", this);
    }
    if (!p)
    {
        return BEE_CORRUPTION;
    }

    m_pRenderer = p;

    return BEE_SUCCESS;
}