#pragma once

namespace Bee::DX12
{
    class BEE_API IRendererComponent
    {
        BEE_USING_BEE_DX12;

        friend RendererDX;

    protected:
        const RendererDX* GetRenderer() { return m_pRenderer; }

    private:
        Status InitializeComponent(RendererDX*);
        
        const RendererDX* m_pRenderer;
    };
}