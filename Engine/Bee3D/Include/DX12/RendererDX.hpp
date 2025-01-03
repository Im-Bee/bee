#pragma once

#include "DxUtils.hpp"
#include "Device.hpp"
#include "CommandQueue.hpp"
#include "SwapChain.hpp"
#include "Resources.hpp"

namespace Bee::DX12
{
    enum RendererFlags
    {
        DX12_RENDERER_MAKE_WINDOW_FLAG = 0x02,
    };

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API RendererDX : public Problems::IDumpOnException
    {
        BEE_USING_BEE_DX12;
        
        using IWindow = Bee::App::IWindow;

        friend class Device;

    public:
                 RendererDX() : m_pWindow(nullptr) {};
        explicit RendererDX(const uint32_t&);
        explicit RendererDX(IWindow* w) : m_pWindow(w) {};
                 RendererDX(IWindow*, const uint32_t&);

        RendererDX(RendererDX&&) = default;

        ~RendererDX() 
        { 
            this->Destroy(); 
        }

    public:
// Public Methods -------------------------------------------------------------
        Status Initialize();
        void   Update();
        void   Render();
        Status Destroy();

    public:
// Getters --------------------------------------------------------------------
        const IWindow*          GetWindow()       const { return m_pWindow; }
        SharedPtr<CommandQueue> GetCommandQueue() const { return m_pCommandQueue; }

    public:
// Setters --------------------------------------------------------------------
        void SetWindow(IWindow*);

    protected:
// Private Methods ------------------------------------------------------------
        virtual void Dump() override;

    private:
        void   ProcessFlags(const uint32_t&);
        Status LoadPipeline();
        Status LoadAssets();

    private:
        IWindow* m_pWindow = 0;

        SharedPtr<Device>       m_pDevice       = 0;
        SharedPtr<CommandQueue> m_pCommandQueue = 0;
        SharedPtr<SwapChain>    m_pSwapChain    = 0;
        SharedPtr<Resources>    m_pResources    = 0;
    };
#pragma warning(pop)
}
