#include "Bee3D.hpp"

BEE_DX12_CPP;

b_status Bee::DX12::IRendererComponent::InitializeComponent(RendererDX* p)
{
    if (m_pRenderer)
    {
        BEE_LOG(Problems::Warning, L"IRendererComponent (%p): IRendererComponent was already initialized. Proceeding anyway.", this);
    }
    if (!p)
        BEE_RETURN_BAD;

    m_pRenderer = p;
    BEE_RETURN_SUCCESS;
}