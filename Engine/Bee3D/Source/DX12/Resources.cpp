#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

#include "../Include/DX12/CommandQueue.hpp"
#include "../Include/DX12/Device.hpp"
#include "../Include/DX12/Resources.hpp"
#include "../Include/DX12/SwapChain.hpp"

BEE_DX12_CPP;

void Bee::DX12::MeshResources::SetGPUSideBuffer(ComPtr<ID3D12Resource> pResource)
{
    m_pGPUSideBuffer = pResource;

    m_GPUTrianglesLocation.BufferLocation = m_pGPUSideBuffer->GetGPUVirtualAddress();
    m_GPUTrianglesLocation.StrideInBytes  = sizeof(::Bee::DX12::ColorVertex);
    m_GPUTrianglesLocation.SizeInBytes    = static_cast<UINT>(this->GetSizeInBytes());
}

b_status MeshResources::LoadMeshOnCPU(const wchar_t* wszPath)
{
    enum Modes
    {
        None,
        V,
        F,
    };

    auto fileBuffer = ::Bee::App::Manager::Get().ReadFile(wszPath);

    if (fileBuffer.Size == 0)
    {
        return BEE_CORRUPTION;
    }

    Vector<XMVECTOR> vectors;

    Modes currentMode = None;
    b_usize lineLenght = 0;
    for (b_usize i = 0; i < fileBuffer.Size; ++i, ++lineLenght)
    {
        const char& c = fileBuffer.Buffer[i];

        if (lineLenght == 0)
        {
            if (ToLower(c) == 'v' && ToLower(*(&c + 1)) == ' ')
            {
                currentMode = V;
                continue;
            }

            if (ToLower(c) == 'f' && ToLower(*(&c + 1)) == ' ')
            {
                currentMode = F;
                continue;
            }

            currentMode = None;
            continue;
        }

        if (c != '\n' && c != '\0')
        {
            continue;
        }

        if (currentMode == V)
        {
            vectors.Push(Move(XMVECTOR()));
            float* xyz[] = { &vectors.GetBack().m128_f32[0], &vectors.GetBack().m128_f32[1], &vectors.GetBack().m128_f32[2] };
            vectors.GetBack().m128_f32[3] = 1.f;

            b_usize j = 1;
            for (int8_t k = 0; k < 3; ++k)
            {
                if (fileBuffer.Buffer[i - lineLenght + j] == '\0')
                {
                    break;
                }

                while (fileBuffer.Buffer[i - lineLenght + j] == ' ')
                {
                    ++j;
                }

                ScanLine(&fileBuffer.Buffer[i - lineLenght + j],
                         lineLenght - j,
                         "%f",
                         *xyz[k]);

                while (fileBuffer.Buffer[i - lineLenght + j] != ' '  &&
                       fileBuffer.Buffer[i - lineLenght + j] != '\n' &&
                       fileBuffer.Buffer[i - lineLenght + j] != '\0')
                {
                    ++j;
                }
            }
        }

        if (currentMode == F)
        {
            m_vCPUTriangles.Push(TriangleColor());
            int32_t p0, p1, p2;
            int32_t* xyz[] = { &p0, &p1, &p2 };

            b_usize j = 1;
            for (int8_t k = 0; k < 3; ++k)
            {
                if (fileBuffer.Buffer[i - lineLenght + j] == '\0')
                {
                    break;
                }

                while (fileBuffer.Buffer[i - lineLenght + j] == ' ')
                {
                    ++j;
                }

                ScanLine(&fileBuffer.Buffer[i - lineLenght + j],
                         lineLenght - j,
                         "%d",
                         *xyz[k]);

                while (fileBuffer.Buffer[i - lineLenght + j] != ' '  &&
                       fileBuffer.Buffer[i - lineLenght + j] != '\n' &&
                       fileBuffer.Buffer[i - lineLenght + j] != '\0')
                {
                    ++j;
                }
            }

            auto& newTriangle = m_vCPUTriangles.GetBack();

            BEE_LOG(Debug::Info, L"%d %d %d", p0, p1, p2);

            newTriangle.p[0] = ColorVertex(vectors[p0 - 1], XMVECTOR({ 1.f, 0.f, 1.f, 1.f }));
            newTriangle.p[1] = ColorVertex(vectors[p1 - 1], XMVECTOR({ 0.f, 1.f, 1.f, 1.f }));
            newTriangle.p[2] = ColorVertex(vectors[p2 - 1], XMVECTOR({ 0.f, 0.f, 1.f, 1.f }));
        }

        lineLenght = -1;
    }

    if (!m_vCPUTriangles.GetSize())
    {
        return BEE_CORRUPTION;
    }

    auto rotation(XMMatrixRotationY(XMConvertToRadians(45.f)));
    auto scaled(XMMatrixScaling(5.f, 5.f, 5.f));
    auto pos(XMMatrixTranslation(0.f, 0.f, 1.65f));
    auto perspective(XMMatrixPerspectiveFovLH(XMConvertToRadians(59.f), 900.f / 700.f, 0.1f, 1000.0f));

    auto transform(XMMatrixMultiply(rotation, scaled));
    transform = XMMatrixMultiply(transform, pos);
    transform = XMMatrixMultiply(transform, perspective);

    for (b_usize i = 0; i < m_vCPUTriangles.GetSize(); ++i)
    {
        auto& c(m_vCPUTriangles[i]);

        c.p[0].Position = XMVector4Transform(c.p[0].Position, transform);
        c.p[1].Position = XMVector4Transform(c.p[1].Position, transform);
        c.p[2].Position = XMVector4Transform(c.p[2].Position, transform);

        BEE_LOG(Debug::Info, L"%f %f %f _ %f %f %f _ %f %f %f", c.p[0].Position.m128_f32[0], c.p[0].Position.m128_f32[1], c.p[0].Position.m128_f32[2], c.p[1].Position.m128_f32[0], c.p[1].Position.m128_f32[1], c.p[1].Position.m128_f32[2], c.p[2].Position.m128_f32[0], c.p[2].Position.m128_f32[1], c.p[2].Position.m128_f32[2]);
    }

    m_bGPUUploadWait = true;

    return BEE_SUCCESS;
}

void MemoryManager::AllocateVerticesBufferOnGPU(VertexResource* pUninitalizedVertexBuffer)
{
    ComPtr<ID3D12Resource> pResource(nullptr);

    D3D12_HEAP_PROPERTIES properties;
    properties.Type                 = D3D12_HEAP_TYPE_UPLOAD;
    properties.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    properties.CreationNodeMask     = 1;
    properties.VisibleNodeMask      = 1;

    D3D12_RESOURCE_DESC desc;
    desc.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment          = 0;
    desc.Width              = pUninitalizedVertexBuffer->GetSizeInBytes();
    desc.Height             = 1;
    desc.DepthOrArraySize   = 1;
    desc.MipLevels          = 1;
    desc.Format             = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags              = D3D12_RESOURCE_FLAG_NONE;

    B_WIN_HANDLE_FAILURE_BEG(m_pDevice->m_pDevice->CreateCommittedResource(&properties, 
                                                                           D3D12_HEAP_FLAG_NONE, 
                                                                           &desc,   
                                                                           D3D12_RESOURCE_STATE_GENERIC_READ, 
                                                                           nullptr, 
                                                                           IID_PPV_ARGS(&pResource)))
        throw ::Bee::Debug::Exception(L"Create committed pResource", BEE_COLLECT_DATA_ON_EXCEPTION());
    B_WIN_HANDLE_FAILURE_END;

    pUninitalizedVertexBuffer->SetGPUSideBuffer(pResource);

    m_vResources.Push(pUninitalizedVertexBuffer);
}

void Bee::DX12::MemoryManager::UploadBuffers()
{
    for (b_usize i = 0; i < m_vResources.GetSize(); ++i) 
    {
        if (m_vResources[i]->m_bGPUUploadWait)
        {
            MeshResources& c(*m_vResources[i]);

            D3D12_RANGE r(0, 0);
            void* pData;
            c.m_pGPUSideBuffer->Map(0, &r, &pData);

            memset(pData, 0, c.GetSizeInBytes());
            memcpy(pData, reinterpret_cast<void*>(&c.GetCPUVertices().GetFront()), c.GetSizeInBytes());

            c.m_bGPUUploadWait = false;
        }
    }
}

void MemoryManager::BindDevice(SharedPtr<Device> device)
{
    m_pDevice = device;
}
