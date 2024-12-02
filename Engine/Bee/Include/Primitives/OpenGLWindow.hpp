#pragma once

namespace Bee::App::Primitives
{
    class BEE_API OpenGLWindow : public Bee::App::IWindow
    {
    public:
        OpenGLWindow(Bee::App::WindowProperties wp = Bee::App::WindowProperties(L"AnEmptyWindow", L"Window", Rectangle(900, 700))) : IWindow(wp) {};

        ~OpenGLWindow() { this->Destroy(); }

    public:
        virtual Bee::Utils::b_status Initialize() override;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }

    private:
        Bee::Utils::b_status SetUpOpenGLContext();

    private:
        HDC   m_HDC = NULL;
        HGLRC m_HRC = NULL;
    };
}