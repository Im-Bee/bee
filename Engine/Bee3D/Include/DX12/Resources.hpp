#pragma once

#include "DxUtils.hpp"

namespace Bee::DX12
{
#pragma warning(push)
    // Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Resources : public IRendererComponent
    {
        BEE_USING_BEE_DX12;

        friend class Device;

    public:
        Resources() = default;
        ~Resources() = default;

        Resources(Resources&&) = default;

// Public Methods -------------------------------------------------------------
    public:
        Status LoadMesh(const wchar_t* wszPath);

    private:
        ComPtr<ID3D12RootSignature> m_pRootSignature = 0;
        ComPtr<ID3D12PipelineState> m_pPipelineState = 0;
    };
#pragma warning(pop)
}
