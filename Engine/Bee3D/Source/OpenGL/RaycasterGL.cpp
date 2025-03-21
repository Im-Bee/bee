#include "Bee3D.hpp"
#include "../Include/OpenGL/RaycasterGL.hpp"

using namespace Bee;
using namespace Bee::GL;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;

void Bee::GL::RaycasterRenderer::LoadMeshFromObj(const wchar_t* wszPath)
{
    enum Modes
    {
        None,
        V,
        F,
    };

    auto fileBuffer = ::Bee::App::Manager::Get().ReadFile(wszPath);
    Vector<Vec3f> vectors;

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

        if (c != '\n')
        {
            continue;
        }

        if (currentMode == V)
        {
            vectors.Push(Move(Vec3f()));
            float* xyz[] = { &vectors.GetBack().x, &vectors.GetBack().y, &vectors.GetBack().z };

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
            m_vTriangles.Push(Move(Triangle3f()));
            b_usize p0, p1, p2;
            b_usize* xyz[] = { &p0, &p1, &p2 };

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

            auto& newTriangle = m_vTriangles.GetBack();

            newTriangle.p0 = vectors[p0 - 1];
            newTriangle.p1 = vectors[p1 - 1];
            newTriangle.p2 = vectors[p2 - 1];

            Mat3x3f scale(3000.f,  0.f,  0.f,
                           0.f, 3000.f,  0.f,
                           0.f,  0.f, 3000.f);

            MultiplyMat3x3Vec3(scale, newTriangle.p0, newTriangle.p0);
            MultiplyMat3x3Vec3(scale, newTriangle.p1, newTriangle.p1);
            MultiplyMat3x3Vec3(scale, newTriangle.p2, newTriangle.p2);
            
            newTriangle.p0 += Vec3f(0.f, 0.f, 350.f);
            newTriangle.p1 += Vec3f(0.f, 0.f, 350.f);
            newTriangle.p2 += Vec3f(0.f, 0.f, 350.f);
            
            BEE_LOG(Debug::Info, L"%d %d %d (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)", p0, p1, p2, newTriangle.p0.x, newTriangle.p0.y, newTriangle.p0.z, newTriangle.p1.x, newTriangle.p1.y, newTriangle.p1.z, newTriangle.p2.x, newTriangle.p2.y, newTriangle.p2.z);
        }

        lineLenght = -1;
    }
}

b_status RaycasterRenderer::Initialize()
{
    if (BEE_IS_COULDNT_DO(m_Window.Initialize()))
    {
        return BEE_CORRUPTION;
    }

    if (BEE_IS_COULDNT_DO(m_Window.Show()))
    {
        return BEE_CORRUPTION;
    }

    if (BEE_IS_COULDNT_DO(LoadPipeline()))
    {
        return BEE_CORRUPTION;
    }

    return BEE_SUCCESS;
}

void RaycasterRenderer::Update()
{
    if (BEE_IS_COULDNT_DO(ResizeScene()))
    {
        throw ::Bee::Debug::Exception(L"Could't resize!", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
}

void RaycasterRenderer::Render()
{
    if (!m_pMainCamera.Get())
    {
        return;
    }

    for (b_usize i = 0; i < m_vPixels.GetCapacity(); ++i)
    {
        m_vPixels[i] = 0;
    }

    const auto& width     = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height    = static_cast<GLsizei>(m_WindowDim.y);
    const auto& cameraPos = m_pMainCamera->GetPos();
    auto rotationMat(CreateYRotationMat3x3(m_pMainCamera->GetYRotation()));
    Vec3f planeVec(-width * .5f, height * .5f, 100.f);

    for (b_isize i = 0; i < height; ++i)
    {
        for (b_isize k = 0; k < width; ++k)
        {
            Vec3f p(planeVec);
            p.x += k;
            p.y -= i;

            MultiplyMat3x3Vec3(rotationMat, p, p);
            p += cameraPos;

            Vec3f vector(0.f, 0.f, 100.f);
            MultiplyMat3x3Vec3(rotationMat, vector, vector);

            const auto& xCoord = p.x;
            const auto& yCoord = p.y;
            const auto& zCoord = p.z;

            auto hit = CastRay(p, vector);

            // Crosshair
            if (static_cast<int32_t>(xCoord) == 0.f || static_cast<int32_t>(yCoord) == 0.f)
            {
                PaintPixel(Vec2f(static_cast<float>(k), static_cast<float>(height - i - 1)), Vec3Byte(255, 255, 255));
                continue;
            }

            if (hit.Entry != BEE_INVALID_VECTOR_3F)
            {
                PaintPixel(Vec2f(static_cast<float>(k), static_cast<float>(height - i - 1)), Vec3Byte(255, 255, 255));
                continue;
            }

            // if (xCoord > 0.f && yCoord > 0.f)
            // {
            //     PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(255, 0, 0));
            // }
            // else if (xCoord > 0.f && yCoord < 0.f)
            // {
            //     PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(0, 255, 0));
            // }
            // else if (xCoord < 0.f && yCoord > 0.f)
            // {
            //     PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(0, 0, 255));
            // }
            // else
            // {
            //     PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(255, 255, 0));
            // }
        }
    }

    BEE_GL(glDrawPixels(width, 
                        height, 
                        GL_RGBA, 
                        GL_UNSIGNED_BYTE, 
                        &m_vPixels));

    if (!SwapBuffers(m_Window.GetHDC()))
    {
        B_WIN_REPORT_FAILURE();
        throw ::Bee::Debug::Exception(L"Could't swap buffers!", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
}

b_status RaycasterRenderer::Destroy()
{
    if (m_Window.GetHandle())
    {
        m_Window.Destroy();
    }

    return BEE_SUCCESS;
}

b_status RaycasterRenderer::LoadPipeline()
{
    BEE_GL(glShadeModel(GL_SMOOTH));
    
    BEE_GL(glClearColor(0.f, 0.f, 0.f, 1.f));
    BEE_GL(glClearDepth(1.));

    BEE_GLEW(glewInit());

    if (BEE_IS_COULDNT_DO(ResizeScene()))
    {
        return BEE_CORRUPTION;
    }

    return BEE_SUCCESS;
}

b_status RaycasterRenderer::ResizeScene()
{
    m_WindowDim = m_Window.GetCurrentDimensions();
    const auto& width  = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height = static_cast<GLsizei>(m_WindowDim.y);

    if (width == 0 || height == 0)
    {
        return BEE_CORRUPTION;
    }

    m_vPixels.SetCapacity(static_cast<b_usize>(4) * width * height);

    BEE_GL(glViewport(0, 0, width, height));

    BEE_GL(glMatrixMode(GL_MODELVIEW));
    BEE_GL(glLoadIdentity());

    BEE_GL(gluOrtho2D(0., 
                      static_cast<double>(width), 
                      static_cast<double>(height), 
                      0.));

    return BEE_SUCCESS;
}

RayHit RaycasterRenderer::CastRay(const Vec3f& origin, const Vec3f& rayVector)
{
    RayHit result = {
        .Entry = BEE_INVALID_VECTOR_3F,
        .Exit  = BEE_INVALID_VECTOR_3F,
    };

    for (b_usize i = 0; i < m_vTriangles.GetSize(); ++i)
    {
        result.Entry = RayIntersectsTriangle(origin, rayVector, m_vTriangles[i]);

        if (result.Entry != BEE_INVALID_VECTOR_3F)
        {
            break;
        }
    }

    return result;
}

Vec3f RaycasterRenderer::RayIntersectsTriangle(const Vec3f&      origin,
                                               const Vec3f&      direction,
                                               const Triangle3f& triangle)
{
    Vec3f edge1 = triangle.p1 - triangle.p0;
    Vec3f edge2 = triangle.p2 - triangle.p0;

    Vec3f rayCrossEdge2 = direction.CrossProduct(edge2);
    float det = edge1.DotProduct(rayCrossEdge2);

    if (fabs(det) < BEE_EPSILON)
    {
        // It's parallel to the triangle
        return BEE_INVALID_VECTOR_3F;
    }

    float invDet = 1.0f / det;
    Vec3f s = origin - triangle.p0;
    float u = invDet * s.DotProduct(rayCrossEdge2);

    if (u < 0.0f ||  u > 1.0f)
    {
        return BEE_INVALID_VECTOR_3F;
    }

    Vec3f sCrossEdge1 = s.CrossProduct(edge1);
    float v = invDet * direction.DotProduct(sCrossEdge1);

    if (v < 0.0f || u + v > 1.0f)
    {
        return BEE_INVALID_VECTOR_3F;
    }

    float t = invDet * edge2.DotProduct(sCrossEdge1);

    if (t > BEE_EPSILON)
    {
        return Vec3f(1.f, 1.f, 1.f);
    }
    else
    {
        return BEE_INVALID_VECTOR_3F;
    }
}

void RaycasterRenderer::PaintPixel(const Vec2f& Coords, const Vec3Byte& Color)
{
    const auto& width  = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height = static_cast<GLsizei>(m_WindowDim.y);

    const auto heightIndex = static_cast<b_usize>(Coords.y) * width * 4;
    const auto widhtIndex  = static_cast<b_usize>(Coords.x) * 4;

    m_vPixels[heightIndex + widhtIndex    ] = Color.x;
    m_vPixels[heightIndex + widhtIndex + 1] = Color.y;
    m_vPixels[heightIndex + widhtIndex + 2] = Color.z;
    m_vPixels[heightIndex + widhtIndex + 3] = 255;
}