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
    constexpr float planeCameraDist = 260.f;
    const auto& width  = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height = static_cast<GLsizei>(m_WindowDim.y);
    const float planeTopLeftX = width  * 0.5f,
                planeTopLeftY = height * 0.5f;

    BEE_GL(glClear(GL_COLOR_BUFFER_BIT));

    for (b_isize i = 0; i < height; ++i)
    {
        for (b_isize k = 0; k < width; ++k)
        {
            float pitchY = (-i / planeCameraDist) * BEE_DEG_TO_RADIAN;
            float pitchX = (-k / planeCameraDist) * BEE_DEG_TO_RADIAN;

            auto hit = CastRay(0.f, 0.f, 0.f, pitchY, pitchX);

            if (hit.Entry != BEE_INVALID_VECTOR_3F)
            {
                PaintPixel(Vec2f(k, i), Vec3Byte(255, 255, 255));
            }
            else
            {
                PaintPixel(Vec2f(k, i), Vec3Byte(0, 0, 0));
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

RayHit RaycasterRenderer::CastRay(const float& x0,
                                  const float& y0,
                                  const float& z0, 
                                  const float& pitchY, 
                                  const float& pitchX)
{
    RayHit result = {
        .Entry = BEE_INVALID_VECTOR_3F,
        .Exit  = BEE_INVALID_VECTOR_3F,
    };

    Vec3f      origin    = Vec3f(x0, y0, z0);
    Vec3f      rayVector = Vec3f(sinf(pitchX) * 500.f, sinf(pitchY) * 500.f, cosf(pitchX) * 500.f);
    Triangle3f triangle  = Triangle3f(Vec3f(0.f, 0.f, 500.f), Vec3f(2.f, 0.f, 500.f), Vec3f(2.f, 1.f, 550.f));

    // Check is it a hit
    result.Entry = RayIntersectsTriangle(origin, rayVector, triangle);

    return result;
}

Vec3f RaycasterRenderer::RayIntersectsTriangle(const Vec3f& origin,
                                               const Vec3f& rayVector,
                                               const Triangle3f& triangle)
{
    Vec3f edge1 = triangle.p2 - triangle.p1;
    Vec3f edge2 = triangle.p3 - triangle.p1;

    Vec3f rayCrossEdge2 = rayVector.CrossProduct(edge2);
    float det = edge1.DotProduct(rayCrossEdge2);

    if (det > -BEE_EPSILON && det < BEE_EPSILON)
    {
        // It's parallel to the triangle
        return BEE_INVALID_VECTOR_3F;
    }

    float invDet = 1.0f / det;
    Vec3f s = origin - triangle.p1;
    float u = invDet * s.DotProduct(rayCrossEdge2);

    if ((u < 0.0f && fabsf(u) > BEE_EPSILON) ||  (u > 1.0f && fabsf(u - 1.f) > BEE_EPSILON))
    {
        return BEE_INVALID_VECTOR_3F;
    }

    Vec3f sCrossEdge1 = s.CrossProduct(edge1);
    float v = invDet * edge2.DotProduct(sCrossEdge1);

    if ((v < 0.0f && fabsf(u) > BEE_EPSILON) || (u + v > 1.0f && fabsf(u + v - 1.f) > BEE_EPSILON))
    {
        return BEE_INVALID_VECTOR_3F;
    }

    float t = invDet * edge2.DotProduct(sCrossEdge1);

    if (t > BEE_EPSILON)
    {
        return Vec3f(origin + rayVector * t);
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