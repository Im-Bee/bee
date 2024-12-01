#pragma once

#include <atomic>
#include <chrono>
#include <queue>
#include <thread>

#ifndef BEE_API
#	define BEE_API 
#endif // !BEE_API

namespace Bee::Problems
{
    enum Severity
    {
        Bee             = 0x00,
        Info            = (Bee + 1),
        Warning         = (Info + 1),
        Error           = (Warning + 1),
        SmartPointers   = (Error + 1),
        Allocators      = (SmartPointers + 1),
        DirectX         = (Allocators + 1),
    };

    typedef std::chrono::system_clock               LoggerClock;
    typedef std::chrono::time_point<LoggerClock>    LoggerTimePoint;
    typedef std::vector<Severity>                   IgnoreList;

    constexpr const std::chrono::milliseconds LogTimeOutMS(100);

    struct LogStamp
    {
        const Severity          Severity;
        wchar_t*                Message;
        const LoggerTimePoint   Time;
    };

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Logger
    {
        using Thread      = std::thread;
        using ABool       = std::atomic_bool;
        using LoggerQueue = std::queue<LogStamp>;

        Logger();

    public:
        ~Logger()
#ifdef _DEBUG
            throw()
#endif // _DEBUG
            ;

        Logger(const Logger&) = delete;
        Logger(Logger&&)      = delete;

        static Logger& Get();

    public:
        void SetPath(const wchar_t* szPath);
        void SetIgnore(IgnoreList&& list);

    public:
        void Log(Severity&& sev, const wchar_t* format, ...);

    private:
        void Work();
        bool ProcessStamp(LogStamp&);
        const wchar_t* GetTag(const Severity&);

    private:
        IgnoreList      m_vSuppressed;
        const wchar_t*  m_szTargetFile;

        ABool           m_bLoop;
        Thread          m_tMainLoop;
        LoggerQueue     m_StampQueue;

        static Logger*  m_pInstance;
    };
#pragma warning(pop)
}