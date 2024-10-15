#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

b_status Renderer::Initialize()
{
    if (!B_IS_OKAY(m_Window->Initialize()))
        B_RETURN_FAIL;

    if (!B_IS_OKAY(m_Window->Show()))
        B_RETURN_FAIL;

    ComPtr<IDXGIFactory> factory = 0;
    B_DXGI_THROW_IF_FAIL(CreateDXGIFactory(IID_PPV_ARGS(&factory)));

    if (!B_IS_OKAY(m_Device.Create(factory)))
        B_RETURN_BAD;

    B_RETURN_SUCCESS;
}

void Renderer::Update()
{
}

void Renderer::Render()
{
}

b_status Renderer::Destroy()
{
    B_RETURN_SUCCESS;
}
