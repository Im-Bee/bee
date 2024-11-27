#include "Bee3D.hpp"

BEE_DX12_CPP;

b_status Bee::DX12::IComponent::InitializeComponent(Renderer* p)
{
    if (m_pRenderer)
    {
        BEE_LOG(Problems::Warning, L"IComponent (%p): IComponent was already initialized. Proceeding anyway.", this);
    }
    if (!p)
        B_RETURN_BAD;

    m_pRenderer = p;
    B_RETURN_SUCCESS;
}