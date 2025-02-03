#include "Bee3D.hpp"

using namespace ::Bee;
using namespace ::Bee::DX12;

void ::Bee::DX12::DirectXLoggingCallback(D3D12_MESSAGE_CATEGORY Category,
                                         D3D12_MESSAGE_SEVERITY Severity,
                                         D3D12_MESSAGE_ID ID,
                                         LPCSTR pDescription,
                                         void* pContext)
{
    if (Severity == D3D12_MESSAGE_SEVERITY_WARNING)
    {
        BEE_LOG(Debug::Warning, L"%S", pDescription);
        return;
    }

    if (Severity < D3D12_MESSAGE_SEVERITY_WARNING)
    {
        BEE_LOG(Debug::Error, L"%S", pDescription);
        return;
    }

    BEE_LOG(Debug::DirectX, L"%S", pDescription);
}

const D3D12_INPUT_ELEMENT_DESC ColorVertex::VertexElementsDesc[2] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    {    "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};