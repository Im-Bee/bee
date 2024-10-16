#pragma once

namespace Bee::DX12
{
    class BEE_API Device 
    {
        template<class T> using ComPtr = Bee::Utils::ComPtr<T>;
                          using Status = Bee::Utils::b_status;

        ComPtr<ID3D12Device> m_pDevice;

    public:
        Device() = default;
        Device(int a) {};
        ~Device() = default;

        Device(Device&&) = default;

    public:
        Status Create(ComPtr<IDXGIFactory> factory);

    };
}
