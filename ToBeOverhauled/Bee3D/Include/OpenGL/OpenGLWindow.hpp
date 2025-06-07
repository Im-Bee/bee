#pragma once

namespace Bee::GL
{
    class BEE_API OpenGLWindow : public Bee::App::IWindow
    {
        using IWindow   = ::Bee::App::IWindow;
        using Status    = ::Bee::Utils::b_status;
        using Rectangle = ::Bee::Utils::Rectangle;
        
        friend class RaycasterRenderer;
        
    public:
        OpenGLWindow(::Bee::App::WindowProperties wp = ::Bee::App::WindowProperties(L"OpenGLWindow",
                                                                                    Rectangle(1200, 700))) 
        : IWindow(wp)
        {};

        ~OpenGLWindow() = default;

    public:
        virtual Status Initialize() override;
        virtual Status Destroy() override;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }

    public:
        const HDC& GetHDC() const { return m_HDC; }

    private:
        Status SetUpOpenGLContext();
        Status ReleaseOpenGLContext();

    private:
        HDC   m_HDC = NULL;
        HGLRC m_HRC = NULL;
    };
}