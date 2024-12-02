#pragma once

namespace Bee::App::Primitives
{
    class BEE_API EmptyWindow : public Bee::App::IWindow
    {
    public:
        EmptyWindow(Bee::App::WindowProperties wp = Bee::App::WindowProperties(L"AnEmptyWindow", L"Window", Rectangle(900, 700))) : IWindow(wp) {};

        ~EmptyWindow() { this->Destroy(); }

    public:
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }
    };
}