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

    struct BEE_API FileData
    {
        const char*                   Buffer;
        Bee::Utils::Memory::b_uintmem Size;


        ~FileData();
    };

    class BEE_API Manager
    {
        friend Bee::App::IWindow;
        using Status      = Bee::Utils::b_status;
        using WindowsList = Bee::Utils::Memory::UnorderedList<Bee::App::IWindow*>;
        
        Manager() = default;

    public:
        ~Manager();

        Manager(Manager&&) = delete;
        Manager(const Manager&) = default;

        static Manager& Get();

// Public Methods -------------------------------------------------------------
    public:
        FileData ReadFile(const wchar_t* szPath);

// Getters --------------------------------------------------------------------
    public:
        const IWindow*        GetMainWindow()        const;
                              
        const uint64_t&       GetWindowsAmount()     const { return m_Windows.GetSize(); }

        Bee::Utils::Vec2<int> GetMonitorResolution() const;

// Private Methods ------------------------------------------------------------
    public:
        void CloseApplication();

    private:
        uint64_t Register(IWindow*);
        Status UnRegister(IWindow*);

        void Quit();

    private:
        static Manager* m_pInstance;

        bool        m_bQuit = false;
        WindowsList m_Windows = {};
        uint64_t    m_uWindowsRollingIndex = 0;

    };
}
