#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
    class Renderer;

    class BEE_API IComponent
    {
        friend Renderer;

        using Status = Bee::Utils::b_status;

    protected:
        const Renderer* GetRenderer() { return m_pRenderer; }

    private:
        Status InitializeComponent(Renderer* p);

    private:
        const Renderer* m_pRenderer;
    };
}