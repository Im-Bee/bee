#pragma once

namespace Bee::App::Primitives
{
    class BEE_API EmptyWindow : public Bee::App::IWindow
    {
        const WindowProperties m_BaseSettings;

    public:
        EmptyWindow() : m_BaseSettings(WindowProperties()) {};
        explicit EmptyWindow(WindowProperties baseSettings) : m_BaseSettings(baseSettings) {};
        ~EmptyWindow()
        {
            this->Destroy();
        }

        // EmptyWindow(EmptyWindow&&) noexcept;
        // EmptyWindow(const EmptyWindow&) noexcept;

    public:
        virtual Bee::Utils::b_status Initialize() override;

        virtual Bee::Utils::b_status Show() override;

        virtual Bee::Utils::b_status Hide() override;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }
    };
}