#pragma once

#include "Device.hpp"

namespace Bee::DX12
{
    void DirectXLoggingCallback(
        D3D12_MESSAGE_CATEGORY,
        D3D12_MESSAGE_SEVERITY,
        D3D12_MESSAGE_ID,
        LPCSTR,
        void*);

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Renderer
    {
        template<class T> using SharedPtr = Bee::Utils::SharedPtr<T>;
                          using Status    = Bee::Utils::b_status;
                          using IWindow   = Bee::App::IWindow;

        IWindow* m_Window;

        SharedPtr<Device> m_pDevice = 0;

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

    public:


    private:


    };
#pragma warning(pop)
}
