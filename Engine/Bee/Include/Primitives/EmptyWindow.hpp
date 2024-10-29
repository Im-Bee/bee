#pragma once

namespace Bee::App::Primitives
{
    class BEE_API EmptyWindow : public Bee::App::IWindow
    {
    public:
                 EmptyWindow() = default;
        explicit EmptyWindow(WindowProperties baseSettings) : IWindow(baseSettings) {};

                ~EmptyWindow() { this->Destroy(); }

    public:
        virtual Bee::Utils::b_status Initialize() override;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }
    };
}