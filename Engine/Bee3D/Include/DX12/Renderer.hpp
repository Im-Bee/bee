#pragma once

#include "DxUtils.hpp"
#include "Device.hpp"
#include "CommandQueue.hpp"
#include "SwapChain.hpp"

namespace Bee::DX12
{
    void DirectXLoggingCallback(
        D3D12_MESSAGE_CATEGORY,
        D3D12_MESSAGE_SEVERITY,
        D3D12_MESSAGE_ID,
        LPCSTR,
        void*);

    enum RendererFlags
    {
        DX12_RENDERER_MAKE_WINDOW_FLAG = 0x02,
    };

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Renderer : public Problems::IDumpOnException
    {
        template<class T> using SharedPtr = Bee::Utils::SharedPtr<T>;
                          using Status    = Bee::Utils::b_status;
                          using IWindow   = Bee::App::IWindow;

        friend Device;

    public:
                 Renderer() : m_pWindow(nullptr) {};
                 Renderer(decltype(__nullptr)) : m_pWindow(nullptr) {};
        explicit Renderer(const uint32_t&);
        explicit Renderer(IWindow* w) : m_pWindow(w) {};
                 Renderer(IWindow*, const uint32_t&);

        Renderer(Renderer&&) = default;

        ~Renderer() { this->Destroy(); }

// Public Methods -------------------------------------------------------------
    public:
        Status Initialize();
        void   Update();
        void   Render();
        Status Destroy();

// Getters --------------------------------------------------------------------
    public:
        const IWindow* GetWindow() const { return m_pWindow; }

// Setters --------------------------------------------------------------------
    public:
        void SetWindow(IWindow*);

// Private Methods ------------------------------------------------------------
    protected:
        SharedPtr<Device>       GetDevice()       const { return m_pDevice; }
        SharedPtr<CommandQueue> GetCommandQueue() const { return m_pCommandQueue; }

        virtual void Dump() override;

    private:
        void   ProcessFlags(const uint32_t&);
        Status LoadPipeline();

    private:
        IWindow* m_pWindow = 0;

        SharedPtr<Device>       m_pDevice       = 0;
        SharedPtr<CommandQueue> m_pCommandQueue = 0;
        SharedPtr<SwapChain>    m_pSwapChain    = 0;
    };
#pragma warning(pop)
}
