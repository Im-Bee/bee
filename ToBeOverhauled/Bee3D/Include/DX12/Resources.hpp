#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
#pragma warning(push)
    // Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API MeshResources 
    {
        BEE_USING_BEE_DX12;

        using VectorTrianglesColor = ::Bee::Utils::Vector<::Bee::DX12::TriangleColor>;
        using b_usize              = ::Bee::Utils::Memory::b_usize;

        friend class Device;
        friend class CommandQueue;
        friend class MemoryManager;

    public:
        
        MeshResources() = default;
        ~MeshResources() = default;

        MeshResources(MeshResources&&) = default;

// Setters --------------------------------------------------------------------
    public:
        
        void SetForTheUploadFlag() { m_bGPUUploadWait = true; }

        void SetGPUSideBuffer(ComPtr<ID3D12Resource> pResource);

// Getters --------------------------------------------------------------------
    public:

        VectorTrianglesColor& GetCPUVertices() { return m_vCPUTriangles; };

        b_usize GetSize() const { return m_vCPUTriangles.GetSize(); }

        b_usize GetSizeInBytes() const { return m_vCPUTriangles.GetByteSize(); }

        const D3D12_VERTEX_BUFFER_VIEW& GetGPUTrianglesView() const { return m_GPUTrianglesLocation; }

// Public Methods -------------------------------------------------------------
    public:

        Status LoadMeshOnCPU(const wchar_t* wszPath);

    private:

        bool m_bGPUUploadWait = false;

        ComPtr<ID3D12RootSignature> m_pRootSignature = 0;
        ComPtr<ID3D12PipelineState> m_pPipelineState = 0;

        VectorTrianglesColor m_vCPUTriangles = {};

        ComPtr<ID3D12Resource>   m_pGPUSideBuffer       = 0;
        D3D12_VERTEX_BUFFER_VIEW m_GPUTrianglesLocation = {};
    };

    class BEE_API MemoryManager : public IRendererComponent
    {
        BEE_USING_BEE_DX12;

        using VertexResource  = ::Bee::DX12::MeshResources;
        using VectorResources = ::Bee::Utils::Vector<VertexResource*>;

        friend class RendererDX;

    public:
        
        MemoryManager() = default;
        ~MemoryManager() = default;

    public:
        
        void AllocateVerticesBufferOnGPU(VertexResource* pUninitalizedVertexBuffer);

        void UploadBuffers();

    private:

        void BindDevice(SharedPtr<Device> device);

    private:

        SharedPtr<Device> m_pDevice    = 0;
        VectorResources   m_vResources = {};
    };
#pragma warning(pop)
}
