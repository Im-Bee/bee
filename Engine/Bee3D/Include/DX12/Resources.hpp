#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
#pragma warning(push)
    // Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API MeshResources : public IRendererComponent
    {
        BEE_USING_BEE_DX12;

        using VectorTrianglesColor = ::Bee::Utils::Vector<::Bee::DX12::TriangleColor>;

        friend class Device;
        friend class CommandQueue;

    public:
        MeshResources() = default;
        ~MeshResources() = default;

        MeshResources(MeshResources&&) = default;

// Public Methods -------------------------------------------------------------
    public:
        Status LoadMesh(const wchar_t* wszPath);

    private:
        ComPtr<ID3D12RootSignature> m_pRootSignature = 0;
        ComPtr<ID3D12PipelineState> m_pPipelineState = 0;

        VectorTrianglesColor m_vCPUTriangles;
    };
#pragma warning(pop)
}
