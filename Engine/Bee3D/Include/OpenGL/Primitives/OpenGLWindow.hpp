#pragma once

namespace Bee::GL::Primitives
{
    class BEE_API OpenGLWindow : public Bee::App::IWindow
    {
        using Status = Bee::Utils::b_status;

    public:
        OpenGLWindow(Bee::App::WindowProperties wp = Bee::App::WindowProperties(L"OpenGLWindow", Rectangle(1200, 700))) : IWindow(wp) {};

        ~OpenGLWindow() = default;

    public:
        virtual Status Initialize() override;
        virtual Status Destroy() override;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }

    private:
        Status SetUpOpenGLContext();
        Status ReleaseOpenGLContext();

    private:
        HDC   m_HDC = NULL;
        HGLRC m_HRC = NULL;
    };
}