#pragma once

#include "OpenGLWindow.hpp"

namespace Bee::GL
{
    struct RayHit
    {
        using Vec3f = ::Bee::Utils::Vec3f;

        Vec3f Entry;
        Vec3f Exit;
    };

    class BEE_API RaycasterRenderer
    {
        using Status      = ::Bee::Utils::b_status;
        using PixelVector = ::Bee::Utils::DynamicArray<unsigned char>;
        using Rectangle   = ::Bee::Utils::Rectangle;
        using Vec2f       = ::Bee::Utils::Vec2<float>;
        using Vec3Byte    = ::Bee::Utils::Vec3<unsigned char>;
        using Vec3f       = ::Bee::Utils::Vec3f;
        using Triangle3f  = ::Bee::Utils::Triangle3f;

    public:
        RaycasterRenderer()
        : m_Window(),
          m_vPixels(),
          m_fRenderDistance(10000.f),
          m_fFov(90.f)
        {};

        ~RaycasterRenderer() = default;

    public:
        OpenGLWindow& GetWindow() { return m_Window; }

    public:
        Status Initialize();
        void   Update();
        void   Render();
        Status Destroy();

    private:
        Status LoadPipeline();
        Status ResizeScene();

        RayHit CastRay(const float& x0,
                       const float& y0,
                       const float& z0,
                       const float& pitchY, 
                       const float& pitchX);

        Vec3f  RayIntersectsTriangle(const Vec3f& origin,
                                     const Vec3f& rayVector,
                                     const Triangle3f& triangle);

        void   PaintPixel(const Vec2f& Coords, const Vec3Byte& Color);

    private:
              OpenGLWindow m_Window          = {};
              PixelVector  m_vPixels         = {};
              Rectangle    m_WindowDim       = Rectangle({ BEE_INFINITY, BEE_INFINITY });
              float        m_fRenderDistance = BEE_INFINITY;
        const float        m_fFov            = BEE_INFINITY;
    };
}