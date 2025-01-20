#pragma once

#include "OpenGLWindow.hpp"

namespace Bee::GL
{
    class BEE_API RendererGL
    {
        using Status = Bee::Utils::b_status;

    public:
        RendererGL() = default;
        ~RendererGL()
        {
            Destroy();
        }

    public:
        OpenGLWindow& GetWindow() { return m_Window; }

    public:
        Status Initialize();
        Status Update();
        Status Render();
        Status Destroy();

    private:
        Status LoadPipeline();
        Status ReSizeScene();

    private:
        ::Bee::GL::OpenGLWindow m_Window = {};

        GLuint m_uShaderProgram = -1;
        GLuint m_uVA = -1;
        GLuint m_uVB = -1;
    };
}