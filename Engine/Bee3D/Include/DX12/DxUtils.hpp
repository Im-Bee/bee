#pragma once

#define BEE_DX12_CPP                       \
    using namespace ::DirectX;             \
    using namespace ::Bee::Utils;          \
    using namespace ::Bee::Utils::Memory;  \
    using namespace ::Bee::DX12;           \

#define BEE_USING_BEE_DX12                                                \
    template<class T> using ComPtr    = ::Bee::Utils::ComPtr<T>;          \
    template<class T> using SharedPtr = ::Bee::Utils::SharedPtr<T>;       \
                      using Status    = ::Bee::Utils::b_status;           \


namespace Bee::DX12
{
    class CommandQueue;
    class SwapChain;
    class MeshResources;
    class MemoryManager;
    class RendererDX;

    void DirectXLoggingCallback(D3D12_MESSAGE_CATEGORY,
                                D3D12_MESSAGE_SEVERITY,
                                D3D12_MESSAGE_ID,
                                LPCSTR,
                                void*);

    struct ColorVertex
    {
        using Vector4 = ::DirectX::XMVECTOR;

        ColorVertex(Vector4 pos = { 0, 0, 0, 0 }, Vector4 col = { 0, 0, 0, 0 })
         : Position(pos), 
           Color(col)
        {};

        Vector4 Position;
        Vector4 Color;

        static const D3D12_INPUT_ELEMENT_DESC VertexElementsDesc[2];
    };

    template<class T>
    struct Triangle
    {
        T p[3];
    };

    typedef ::Bee::DX12::Triangle<::Bee::DX12::ColorVertex> TriangleColor;
}