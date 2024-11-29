#include "Bee3D.hpp"

void Bee::DX12::DirectXLoggingCallback(
    D3D12_MESSAGE_CATEGORY Category,
    D3D12_MESSAGE_SEVERITY Severity,
    D3D12_MESSAGE_ID ID,
    LPCSTR pDescription,
    void* pContext)
{
    BEE_LOG(Problems::Info, L"DirectX: %S", pDescription);
}