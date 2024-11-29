#pragma once

namespace Bee::DX12
{
    class BEE_API IRendererComponent
    {
        BEE_USING_BEE_DX12;

        friend Renderer;

    protected:
        const Renderer* GetRenderer() { return m_pRenderer; }

    private:
        Status InitializeComponent(Renderer*);
        
        const Renderer* m_pRenderer;
    };
}