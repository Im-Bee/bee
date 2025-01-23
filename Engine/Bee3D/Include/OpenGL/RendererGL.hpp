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

        static void Aciton(void* pObj)
        {
            reinterpret_cast<RendererGL*>(pObj)->m_dT += 1.f; 
        }

        static void Aciton2(void* pObj)
        {
            reinterpret_cast<RendererGL*>(pObj)->m_dT -= 1.f;
        }

    private:
        Status LoadPipeline();
        Status ReSizeScene();

    private:
        ::Bee::GL::OpenGLWindow m_Window = {};

        GLuint m_uShaderProgram = -1;
        GLuint m_uVA = -1;
        GLuint m_uVB = -1;

        float  m_dT  = BEE_INFINITY;
    };
}