#pragma once

namespace Bee::App
{
    enum BEE_API CloseAction
    {
        NoWindow,
        StayInBackground,
        NoMainWindow
    };

    constexpr Bee::App::CloseAction OnClose = NoMainWindow;

    class BEE_API Manager
    {
        friend Bee::App::IWindow;
        using Status = Bee::Utils::b_status;
        using WindowsList = Bee::Utils::Memory::UnorderedList<Bee::App::IWindow*>;

        bool        m_bQuit = false;
        WindowsList m_Windows = {};
        uint64_t    m_uWindowsRollingIndex = 0;

        static Manager* m_pInstance;
        
        Manager() = default;

    public:
        ~Manager() = default;

        Manager(Manager&&) = delete;
        Manager(const Manager&) = default;

        static Manager& Get();

    public:
        const IWindow* GetMainWindow() const;

        const uint64_t& GetWindowsAmount() const { return m_Windows.GetSize(); }

    public:
        void CloseApplication();

    private:
        uint64_t Register(IWindow* wnd);

        Status UnRegister(IWindow* wnd);

        void Quit();

    };
}
