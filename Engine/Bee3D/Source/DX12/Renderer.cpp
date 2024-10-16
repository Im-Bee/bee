#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

b_status Renderer::Initialize()
{
    B_LOG(Problems::Info, L"Renderer (%p): Initializing", this);

    if (!B_IS_OKAY(m_Window->Initialize()))
        B_RETURN_FAIL;

    if (!B_IS_OKAY(m_Window->Show()))
        B_RETURN_FAIL;

    m_pDevice = MakeShared<Device>();

    ComPtr<IDXGIFactory> factory = 0;
    B_DXGI_THROW_IF_FAIL(CreateDXGIFactory(IID_PPV_ARGS(&factory)));

    if (!B_IS_OKAY(m_pDevice->Create(factory)))
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
    B_LOG(Problems::Info, L"Renderer (%p): Destroying", this);

    B_RETURN_SUCCESS;
}
