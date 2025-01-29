#include "Bee3D.hpp"

#include "../Include/DX12/RendererDX.hpp"

BEE_DX12_CPP;

b_status Resources::LoadMesh(const wchar_t* wszPath)
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

    Vector<XMVECTOR> vectors;
    Vector<Triangle> triangles;

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
            triangles.Push(Move(Triangle()));
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

            auto& newTriangle = triangles.GetBack();

            newTriangle.p[0] = vectors[p0 - 1];
            newTriangle.p[1] = vectors[p1 - 1];
            newTriangle.p[2] = vectors[p2 - 1];
        }

        lineLenght = -1;
    }

    if (!triangles.GetSize())
    {
        BEE_RETURN_FAIL;
    }

    BEE_RETURN_SUCCESS;
}
