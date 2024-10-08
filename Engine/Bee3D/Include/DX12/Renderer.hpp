#pragma once

#include "Device.hpp"

namespace Bee::DX12
{
    class BEE_API Renderer
    {
        using status = Bee::Utils::b_status;

        Bee::App::IWindow* m_Window;

        Bee::DX12::Device m_Device = {};

    public:
        Renderer() : m_Window(new Bee::App::Primitives::EmptyWindow()) {};
        Renderer(Bee::App::IWindow* wnd) : m_Window(wnd) {};
        ~Renderer()
        {
            this->Destroy();
        }

    public:
        status Initialize();
        void Update();
        void Render();
        status Destroy();

    private:


    };
}
