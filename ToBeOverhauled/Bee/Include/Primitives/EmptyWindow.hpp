#pragma once

namespace Bee::App::Primitives
{
    class BEE_API EmptyWindow : public Bee::App::IWindow
    {
        using Properties = ::Bee::App::WindowProperties;
        using Rectangle  = ::Bee::Utils::Rectangle;

    public:
        EmptyWindow(Properties wp = Properties(L"AnEmptyWindow", Rectangle(900, 700))) 
        : IWindow(wp) 
        {};

        ~EmptyWindow() = default;

    protected:
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }
    };
}