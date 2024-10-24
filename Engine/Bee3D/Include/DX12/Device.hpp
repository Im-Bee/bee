#pragma once

namespace Bee::DX12
{
    class Renderer;

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Device : public IComponent
    {
        template<class T> using ComPtr = Bee::Utils::ComPtr<T>;
                          using Status = Bee::Utils::b_status;

        ComPtr<ID3D12Device> m_pDevice  = 0;
        ComPtr<IDXGIAdapter> m_pAdapter = 0;

    public:
        Device() = default;
        ~Device() = default;

        Device(Device&&) = default;

    public:
        Status Initialize(ComPtr<IDXGIFactory> factory);

    public:
        Status CreateDebugCallback();

    };
#pragma warning(pop)
}
