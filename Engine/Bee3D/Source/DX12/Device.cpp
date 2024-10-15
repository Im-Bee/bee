#include "Bee3D.hpp"

#include "DX12/Renderer.hpp"

BEE_DX12_CPP;

b_status Device::Create(ComPtr<IDXGIFactory> factory)
{
    ComPtr<IDXGIFactory6> factory6 = 0;

    B_WIN_HANDLE_FAILURE_BEG(factory->QueryInterface(IID_PPV_ARGS(&factory6)))
        B_RETURN_BAD;
    B_WIN_HANDLE_FAILURE_END;
    


    B_RETURN_SUCCESS;
}
