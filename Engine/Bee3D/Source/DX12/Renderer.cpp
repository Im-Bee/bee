#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP()

b_status Renderer::Initialize()
{
    if (!B_IS_OKAY(m_Window->Initialize()))
        B_RETURN_FAIL;

    if (!B_IS_OKAY(m_Window->Show()))
        B_RETURN_FAIL;

    if (!B_IS_OKAY(m_Device.CreateDevice()))
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
