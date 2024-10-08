#pragma once

namespace Bee::DX12
{
    class BEE_API Device 
    {
        using status = Bee::Utils::b_status;

        Bee::Utils::ComPtr<ID3D12Device> m_pDevice = 0;

    public:
        Device() = default;
        ~Device() = default;

    public:
        status CreateDevice();

    };
}
