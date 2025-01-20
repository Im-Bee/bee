#pragma once

#include "OpenGLWindow.hpp"

namespace Bee::GL
{
    typedef ::Bee::Utils::Vec3<float> Coords;

    struct RayHit
    {
        Coords Entry;
        Coords Exit;
    };

    class BEE_API RaycasterRenderer
    {
        using PixelVector = ::Bee::Utils::DynamicArray<unsigned char>;
        using Rectangle   = ::Bee::Utils::Rectangle; 
        using Status      = ::Bee::Utils::b_status;

    public:
        RaycasterRenderer()
        : m_Window(),
          m_vPixels(),
          m_RenderDistance(5000.f)
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

    private:
        OpenGLWindow m_Window         = {};
        PixelVector  m_vPixels        = {};
        Rectangle    m_WindowDim      = ::Bee::Utils::Memory::Move(Rectangle({ BEE_INFINITY, BEE_INFINITY }));
        float        m_RenderDistance = BEE_INFINITY;
    };
}