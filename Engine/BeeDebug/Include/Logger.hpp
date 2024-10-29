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
    enum Severity;
    struct LogStamp;

    typedef std::chrono::system_clock               LoggerClock;
    typedef std::chrono::time_point<LoggerClock>    LoggerTimePoint;
    typedef std::vector<Severity>                   SuppressionList;

    constexpr std::chrono::milliseconds LogTimeOutMS(100);

    enum Severity
    {
        Bee             = 0x00,
        Info	        = 0x02,
        Warning         = 0x04,
        Error	        = 0x08,
        SmartPointers   = 0x10,
        Allocators      = 0x20
    };

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
        using thread      = std::thread;
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
        void SetSuppressed(SuppressionList&& list);

    public:
        void Log(Severity&& sev, const wchar_t* format, ...);

    private:
        void Work();
        bool ProcessStamp(LogStamp&);
        const wchar_t* GetTag(const Severity&);

    private:
        SuppressionList m_vSuppressed;
        const wchar_t*  m_szTargetFile;

        ABool           m_bLoop;
        thread          m_tMainLoop;
        LoggerQueue     m_StampQueue;

        static Logger*  m_pInstance;
    };
#pragma warning(pop)
}