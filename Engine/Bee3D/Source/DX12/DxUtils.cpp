#include "Bee3D.hpp"

void Bee::DX12::DirectXLoggingCallback(
    D3D12_MESSAGE_CATEGORY Category,
    D3D12_MESSAGE_SEVERITY Severity,
    D3D12_MESSAGE_ID ID,
    LPCSTR pDescription,
    void* pContext)
{
    if (Severity == D3D12_MESSAGE_SEVERITY_WARNING)
    {
        BEE_LOG(Problems::Warning, L"%S", pDescription);
        return;
    }

    if (Severity < D3D12_MESSAGE_SEVERITY_WARNING)
    {
        BEE_LOG(Problems::Error, L"%S", pDescription);
        return;
    }

    BEE_LOG(Problems::DirectX, L"%S", pDescription);
}