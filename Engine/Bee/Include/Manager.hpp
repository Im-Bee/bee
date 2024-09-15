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

        Manager() = default;

    public:
        ~Manager() = default;

        Manager(Manager&&) = delete;
        Manager(const Manager&) = default;


        static Manager& Get()
        {
            static Manager instance = {};
            return instance;
        }

    public:
        Bee::App::IWindow* GetMainWindow();

        void CloseApplication();

    private:
        uint64_t Register(Bee::App::IWindow* wnd);

        bool UnRegister(Bee::App::IWindow* wnd);

    };
}
