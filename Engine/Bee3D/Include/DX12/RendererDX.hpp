#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
    enum RendererFlags
    {
        DX12_RENDERER_NONE_FLAG = 0x00,
        DX12_RENDERER_MAKE_WINDOW_FLAG = (DX12_RENDERER_NONE_FLAG + 1),
    };

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API RendererDX : public Debug::IOnException
    {
        BEE_USING_BEE_DX12;
        
        using IWindow = ::Bee::App::IWindow;

        friend class Device;

    public:
        explicit RendererDX(const uint32_t& flags   = DX12_RENDERER_NONE_FLAG,
                                   IWindow* pWindow = nullptr);

        RendererDX(      RendererDX&&) = default;
        RendererDX(const RendererDX&)  = delete;

        ~RendererDX();

// Public Methods -------------------------------------------------------------
    public:
        Status Initialize();
        void Update();
        void Render();
        Status Destroy();

// Getters --------------------------------------------------------------------
    public:
        
        const IWindow* GetWindow() const { return m_pWindow; }

        const SharedPtr<CommandQueue>& GetCommandQueue() const { return m_pCommandQueue; }

        const SharedPtr<SwapChain>& GetSwapChain() const { return m_pSwapChain; }

        const SharedPtr<MeshResources>& GetMeshResources() const { return m_pResources; }

// Setters --------------------------------------------------------------------
    public:

        void SetWindow(IWindow*);

// Private Methods ------------------------------------------------------------
    protected:

        virtual void HandleObjects() override;

    private:

        void ProcessFlags(const uint32_t&);
        
        Status LoadPipeline();
        
        Status LoadAssets();

    private:

        IWindow* m_pWindow = 0;

        SharedPtr<Device>        m_pDevice        = 0;
        SharedPtr<CommandQueue>  m_pCommandQueue  = 0;
        SharedPtr<SwapChain>     m_pSwapChain     = 0;
        SharedPtr<MemoryManager> m_pMemoryMenager = 0;
        SharedPtr<MeshResources> m_pResources     = 0;

    };
#pragma warning(pop)
}
