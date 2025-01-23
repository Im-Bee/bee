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
        
        FileData(FileData&& other) noexcept
        : Buffer(other.Buffer),
        Size(other.Size)
        {
            other.Buffer = nullptr;
        }

        ~FileData();

    private:
        FileData(char* pBuffer = nullptr, Usize uSize = 0)
        : Buffer(pBuffer), Size(uSize)
        {};
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

// Public Methods -------------------------------------------------------------
    public:
        FileData ReadFile(const wchar_t* szPath);

// Getters --------------------------------------------------------------------
    public:
        const IWindow*   GetMainWindow()        const;
                         
        const uint64_t&  GetWindowsAmount()     const { return m_Windows.GetSize(); }

              Vec2i      GetMonitorResolution() const;

// Public Methods -------------------------------------------------------------
    public:
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
