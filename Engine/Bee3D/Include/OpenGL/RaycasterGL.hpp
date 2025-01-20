#pragma once

#include "OpenGLWindow.hpp"

namespace Bee::GL
{
    class BEE_API RaycasterRenderer
    {
        using PixelVector = ::Bee::Utils::DynamicArray<unsigned char>;
        using Rectangle   = ::Bee::Utils::Rectangle; 
        using Status      = ::Bee::Utils::b_status;

    public:
        RaycasterRenderer()
        : m_Window(),
          m_vPixels(),
          m_WindowDim(::Bee::Utils::Memory::Move(Rectangle({-1., -1.})))
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

    private:
        OpenGLWindow m_Window;
        PixelVector  m_vPixels;
        Rectangle    m_WindowDim;
    };
}