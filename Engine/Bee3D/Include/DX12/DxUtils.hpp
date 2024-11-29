#pragma once

#define BEE_DX12_CPP            \
    using namespace Bee::Utils; \
    using namespace Bee::DX12;  \

#define BEE_USING_BEE_DX12                                              \
    template<class T> using ComPtr    = Bee::Utils::ComPtr<T>;          \
    template<class T> using SharedPtr = Bee::Utils::SharedPtr<T>;       \
                      using Status    = Bee::Utils::b_status;           \


namespace Bee::DX12
{
    class Renderer;

    void DirectXLoggingCallback(
        D3D12_MESSAGE_CATEGORY,
        D3D12_MESSAGE_SEVERITY,
        D3D12_MESSAGE_ID,
        LPCSTR,
        void*);

}