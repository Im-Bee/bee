#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP()

b_status Device::CreateDevice()
{
    ComPtr<IDXGIFactory> factory = 0;
    ComPtr<IDXGIFactory6> factory6 = 0;

    if (B_WIN_FAILED(CreateDXGIFactory(IID_PPV_ARGS(&factory))))
        B_RETURN_BAD;

    if (B_WIN_FAILED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
        B_RETURN_BAD;
    
    B_RETURN_SUCCESS;
}
