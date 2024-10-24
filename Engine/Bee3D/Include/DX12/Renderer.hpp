#pragma once

#include "Device.hpp"
#include "CommandQueue.hpp"

namespace Bee::DX12
{
    void DirectXLoggingCallback(
        D3D12_MESSAGE_CATEGORY,
        D3D12_MESSAGE_SEVERITY,
        D3D12_MESSAGE_ID,
        LPCSTR,
        void*);

#define BEE_DX12_RENDERER_MAKE_WINDOW_FLAG  ((Bee::DX12::RendererFlags)0x02)

    enum RendererFlags
    {
        MakeWindow = BEE_DX12_RENDERER_MAKE_WINDOW_FLAG,
    };

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Renderer
    {
        template<class T> using SharedPtr = Bee::Utils::SharedPtr<T>;
                          using Status    = Bee::Utils::b_status;
                          using IWindow   = Bee::App::IWindow;

        IWindow* m_pWindow = 0;

        SharedPtr<Device>       m_pDevice = 0;
        SharedPtr<CommandQueue> m_pCommandQueue = 0;

    public:
        Renderer() : m_pWindow(nullptr) {};
        Renderer(decltype(__nullptr)) : m_pWindow(nullptr) {};
        Renderer(const uint32_t&);
        explicit Renderer(IWindow* wnd) : m_pWindow(wnd) {};
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
        SharedPtr<Device> GetDevice() { return m_pDevice; }

    public:
        void SetWindow(IWindow* w) 
        {
            if (m_pDevice.GetPtr())
            {
                B_LOG(Problems::Warning, L"Renderer (%p): SetWindow, window is already set", this);
            }

            m_pWindow = w;
        }

    private:


    };
#pragma warning(pop)
}
