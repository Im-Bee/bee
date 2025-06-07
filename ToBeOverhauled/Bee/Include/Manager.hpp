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
        using Usize = ::Bee::Utils::Memory::b_usize;

        friend class Manager;

        const char* Buffer;
        const Usize Size;

        ~FileData();

        FileData(const FileData& other) noexcept;
        FileData(FileData&& other) noexcept;

    private:
        FileData(char* pBuffer = nullptr, Usize uSize = 0);
    };

    class BEE_API Manager
    {
        using Status      = ::Bee::Utils::b_status;
        using Usize       = ::Bee::Utils::Memory::b_usize;
        using Vec2i       = ::Bee::Utils::Vec2<int32_t>;
        using WindowsList = ::Bee::Utils::UnorderedList<Bee::App::IWindow*>;

        friend ::Bee::App::IWindow;
        
        Manager()
        : m_Windows(),
          m_uWindowsRollingIndex(0)
        {};

    public:
        ~Manager();

        Manager(      Manager&&) = delete;
        Manager(const Manager&)  = default;

        static Manager& Get();

// Getters --------------------------------------------------------------------
    public:
        const IWindow*  GetMainWindow()        const;

        const uint64_t& GetWindowsAmount()     const { return m_Windows.GetSize(); }

              Vec2i     GetMonitorResolution() const;

// Public Methods -------------------------------------------------------------
    public:
        FileData ReadFile(const wchar_t* wszPath);
        
        void CloseApplication();

// Private Methods ------------------------------------------------------------
    private:
        uint64_t Register(IWindow*);
        Status   UnRegister(IWindow*);

        void     Quit();

    private:
        static Manager* m_pInstance;

        bool        m_bQuit                = false;
        WindowsList m_Windows              = {};
        uint64_t    m_uWindowsRollingIndex = -1;
    };
}
