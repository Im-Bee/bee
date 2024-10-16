#pragma once

#include "Device.hpp"

namespace Bee::DX12
{
    class BEE_API Renderer
    {
        using Status  = Bee::Utils::b_status;
        using IWindow = Bee::App::IWindow;

        IWindow* m_Window;

        Bee::Utils::SharedPtr<Device> m_pDevice = 0;

    public:
        Renderer() : m_Window(new Bee::App::Primitives::EmptyWindow()) {};
        Renderer(IWindow* wnd) : m_Window(wnd) {};
        ~Renderer()
        {
            this->Destroy();
        }

    public:
        Status Initialize();
        void Update();
        void Render();
        Status Destroy();

    private:


    };
}
