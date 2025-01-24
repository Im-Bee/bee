#include "Bee3D.hpp"
#include "../Include/OpenGL/RaycasterGL.hpp"

using namespace Bee;
using namespace Bee::GL;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;

b_status RaycasterRenderer::Initialize()
{
    if (BEE_FAILED(m_Window.Initialize()))
    {
        BEE_RETURN_FAIL;
    }

    if (BEE_FAILED(m_Window.Show()))
    {
        BEE_RETURN_FAIL;
    }

    if (BEE_FAILED(LoadPipeline()))
    {
        BEE_RETURN_BAD;
    }

    BEE_RETURN_SUCCESS;
}

void RaycasterRenderer::Update()
{
    if (BEE_FAILED(ResizeScene()))
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

    const auto& width     = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height    = static_cast<GLsizei>(m_WindowDim.y);
    const auto& cameraPos = m_pMainCamera->GetPos();
    auto rotationMat(CreateRotationYMat(m_pMainCamera->GetYRotation()));
    Vec3f planeVec(-width * .5f, height * .5f, 100.f);

    for (b_isize i = 0; i < height; ++i)
    {
        for (b_isize k = 0; k < width; ++k)
        {
            Vec3f p(planeVec);
            p.x += k;
            p.y -= i;

            MatMulVec(rotationMat, p, p);
            p += cameraPos;

            Vec3f vector(0.f, 0.f, 100.f);
            MatMulVec(rotationMat, vector, vector);

            const auto& xCoord = p.x;
            const auto& yCoord = p.y;
            const auto& zCoord = p.z;

            auto hit = CastRay(p, vector);

            // Crosshair
            if (static_cast<int32_t>(xCoord) == 0.f || static_cast<int32_t>(yCoord) == 0.f)
            {
                PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(255, 255, 255));
                continue;
            }

            if (hit.Entry != BEE_INVALID_VECTOR_3F)
            {
                PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(0, 0, 0));
                continue;
            }

            if (xCoord > 0.f && yCoord > 0.f)
            {
                PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(255, 0, 0));
            }
            else if (xCoord > 0.f && yCoord < 0.f)
            {
                PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(0, 255, 0));
            }
            else if (xCoord < 0.f && yCoord > 0.f)
            {
                PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(0, 0, 255));
            }
            else
            {
                PaintPixel(Vec2f(k, height - i - 1), Vec3Byte(255, 255, 0));
            }
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

    BEE_RETURN_SUCCESS;
}

b_status RaycasterRenderer::LoadPipeline()
{
    BEE_GL(glShadeModel(GL_SMOOTH));
    
    BEE_GL(glClearColor(0.f, 0.f, 0.f, 1.f));
    BEE_GL(glClearDepth(1.));

    BEE_GLEW(glewInit());

    if (BEE_FAILED(ResizeScene()))
    {
        BEE_RETURN_BAD;
    }

    BEE_RETURN_SUCCESS;
}

b_status RaycasterRenderer::ResizeScene()
{
    m_WindowDim = m_Window.GetCurrentDimensions();
    const auto& width  = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height = static_cast<GLsizei>(m_WindowDim.y);

    if (width == 0 || height == 0)
    {
        BEE_RETURN_FAIL;
    }

    m_vPixels.SetCapacity(static_cast<b_usize>(4) * width * height);

    BEE_GL(glViewport(0, 0, width, height));

    BEE_GL(glMatrixMode(GL_MODELVIEW));
    BEE_GL(glLoadIdentity());

    BEE_GL(gluOrtho2D(0., 
                      static_cast<double>(width), 
                      static_cast<double>(height), 
                      0.));

    BEE_RETURN_SUCCESS;
}

RayHit RaycasterRenderer::CastRay(const Vec3f& origin, const Vec3f& rayVector)
{
    RayHit result = {
        .Entry = BEE_INVALID_VECTOR_3F,
        .Exit  = BEE_INVALID_VECTOR_3F,
    };

    Triangle3f triangle(Vec3f(   0.f,  100.f, 150.f), 
                        Vec3f( 100.f,  100.f, 150.f), 
                        Vec3f(-100.f, -100.f, 150.f));

    result.Entry = RayIntersectsTriangle(origin, rayVector, triangle);

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

    const auto heightIndex = Coords.y * width * 4;
    const auto widhtIndex  = Coords.x * 4;

    m_vPixels[heightIndex + widhtIndex    ] = Color.x;
    m_vPixels[heightIndex + widhtIndex + 1] = Color.y;
    m_vPixels[heightIndex + widhtIndex + 2] = Color.z;
    m_vPixels[heightIndex + widhtIndex + 3] = 255;
}