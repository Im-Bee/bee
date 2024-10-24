#pragma once

namespace Bee::DX12
{
    class Renderer;

    class BEE_API IComponent
    {
        friend Bee::DX12::Renderer;

        using Status = Bee::Utils::b_status;

        Renderer* m_pRenderer;

    protected:
        Renderer* GetRenderer() { return m_pRenderer; }

    private:
        Status InitializeComponent(Renderer* p)
        {
            if (!p)
                B_RETURN_BAD;
            
            m_pRenderer = p;
            B_RETURN_SUCCESS;
        }
    };
}