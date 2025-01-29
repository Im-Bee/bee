#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

BEE_DX12_CPP;

b_status MeshResources::LoadMesh(const wchar_t* wszPath)
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
        BEE_RETURN_FAIL;
    }

    Vector<XMFLOAT4> vectors;

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
            vectors.Push(Move(XMFLOAT4()));
            float* xyz[] = { &vectors.GetBack().x, &vectors.GetBack().y, &vectors.GetBack().z };
            vectors.GetBack().w = 1.f;

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
            m_vCPUTriangles.Push(Move(TriangleColor()));
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

            newTriangle.p[0] = ColorVertex(vectors[p0 - 1], XMFLOAT4(1.f, 0.f, 1.f, 1.f));
            newTriangle.p[1] = ColorVertex(vectors[p1 - 1], XMFLOAT4(0.f, 1.f, 1.f, 1.f));
            newTriangle.p[2] = ColorVertex(vectors[p2 - 1], XMFLOAT4(0.f, 0.f, 1.f, 1.f));
        }

        lineLenght = -1;
    }

    if (!m_vCPUTriangles.GetSize())
    {
        BEE_RETURN_FAIL;
    }

    BEE_RETURN_SUCCESS;
}
