#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

b_status Device::Create(ComPtr<IDXGIFactory> factory)
{
    B_LOG(Problems::Info, L"Device (%p): Creating", this);

    ComPtr<IDXGIFactory6> factory6 = 0;

    B_DXGI_HANDLE_FAILURE_BEG(factory->QueryInterface(IID_PPV_ARGS(&factory6)))
        B_RETURN_BAD;
    B_DXGI_HANDLE_FAILURE_END;
    
    for (
        uint32_t i = 0;
        factory6->EnumAdapterByGpuPreference(
            i,
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(&m_pAdapter)) != DXGI_ERROR_NOT_FOUND;
        ++i)
    {
        if (B_WIN_SUCCEEDED(D3D12CreateDevice(
            m_pAdapter.Get(),
            D3D_FEATURE_LEVEL_12_0,
            IID_PPV_ARGS(&m_pDevice))))
        {
            B_LOG(Problems::Info, L"Device (%p): Succesfully created a device", this);
            B_RETURN_SUCCESS;
        }
    }

    B_RETURN_BAD;
}

b_status Bee::DX12::Device::CreateDebugCallback()
{
#ifdef _DEBUG
    if (!m_pAdapter.Get() ||
        !m_pDevice.Get())
        B_RETURN_BAD;

    ComPtr<ID3D12InfoQueue1> infoQueue = 0;
    B_DXGI_HANDLE_FAILURE_BEG(m_pDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)))
        B_RETURN_FAIL;
    B_DXGI_HANDLE_FAILURE_END;

    DWORD cookie = 0;
    infoQueue->RegisterMessageCallback(
        Bee::DX12::DirectXLoggingCallback,
        D3D12_MESSAGE_CALLBACK_IGNORE_FILTERS,
        NULL,
        &cookie);

    if (!cookie)
        B_RETURN_FAIL;
#endif // _DEBUG
    
    B_RETURN_SUCCESS;
}
