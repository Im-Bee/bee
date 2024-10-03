#pragma once

#ifndef BEE_API
#	define BEE_API 
#endif // !BEE_API

#include <atomic>
#include <chrono>
#include <queue>
#include <thread>

#define BEE_LOAD_LOGGER() Bee::Problems::Logger::Get()

#define BEE_PROBLEMS_LOGGER_MAX_MESSAGE ((int)255)

#pragma warning(push)
#pragma warning(disable : 4251)

namespace Bee::Problems
{
    enum Severity
    {
        Bee             = 0x00,
        Info	        = 0x02,
        Warning         = 0x04,
        Error	        = 0x08,
        SmartPointers   = 0x10
    };

    struct LogStamp
    {
        const Bee::Problems::Severity Severity;
        wchar_t* Message;
        const std::chrono::time_point<std::chrono::system_clock> Time;
    };

    constexpr std::chrono::milliseconds WriteTimeoutMs(100);
    typedef std::vector<Bee::Problems::Severity> SuppressionList;

    class BEE_API Logger
    {
        std::atomic_bool m_bLoop;
        std::thread m_tMainLoop;
        std::queue<LogStamp> m_StampQueue = {};
        const wchar_t* m_szTargetFile = nullptr;

        SuppressionList m_vSuppressed;

        Logger();

    public:
        ~Logger()
#ifdef _DEBUG
            throw()
#endif // _DEBUG
            ;

        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;

        static Logger& Get()
        {
            static Logger instantce;
            return instantce;
        }

    public:
        void SetPath(const wchar_t* szPath);

        void SetSuppressed(SuppressionList&& list);

    public:
        void Log(
            Severity&& sev,
            const wchar_t* format,
            ...);

    private:
        void Work();

        bool ProcessStamp(LogStamp& ls);

        const wchar_t* GetTag(const Bee::Problems::Severity& s);
    };
}

#pragma warning(pop)