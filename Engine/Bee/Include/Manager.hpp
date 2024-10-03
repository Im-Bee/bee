#pragma once

namespace Bee::App
{
    enum BEE_API CloseAction
    {
        CloseOnNoWindow,
        StayInBackground,
        CloseOnNoMainWindow
    };

    constexpr Bee::App::CloseAction OnClose = CloseOnNoMainWindow;

    class BEE_API Manager
    {
        friend Bee::App::IWindow;

        Bee::Utils::Memory::UnorderedList<Bee::App::IWindow*> m_Windows = {};
        uint64_t m_WindowsRollingIndex = 0;

        static Manager* m_pInstance;
        Manager() = default;

    public:
        ~Manager() = default;

        Manager(Manager&&) = delete;
        Manager(const Manager&) = default;


        static Manager& Get();

    public:
        const Bee::App::IWindow* GetMainWindow();

        const uint64_t& GetWindowsAmount() { return m_Windows.GetSize(); }

        void CloseApplication();

    private:
        uint64_t Register(Bee::App::IWindow* wnd);

        Bee::Utils::b_success UnRegister(Bee::App::IWindow* wnd);

    };
}
