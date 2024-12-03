#pragma once

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
        Status Initialize();
        Status Update();
        Status Render();
        Status Destroy();

    private:
        Status LoadPipeline();
        Status LoadGL();
        Status ReSizeScene();

    private:
        Bee::GL::Primitives::OpenGLWindow m_Window = {};
    };
}