#include "BeeDebug.hpp"

#include <atomic>
#include <chrono>
#include <cstdarg>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace Bee::Debug;

typedef system_clock            LoggerClock;
typedef time_point<LoggerClock> LoggerTimePoint;

constexpr const nanoseconds LogTimeOutMS(1);

struct LogStamp
{
    const Severity        Severity;
          wstring         Message;
    const LoggerTimePoint Time;
};

Logger* Logger::m_pInstance = new Logger();

class Logger::_Impl
{
    friend class Logger;

public:
    bool ProcessStamp(      LogStamp& ls, 
                      const wchar_t*  szTargetPath,
                      const size_t&   uIgnoreListSize,
                            Severity* pIgnoreList)
    {
        using of  = wofstream;
        using wss = wstringstream;

        constexpr const auto timeFormat("{0:%H:%M:%S}");

        if (!szTargetPath)
        {
            return false;
        }

        for (size_t i = 0; i < uIgnoreListSize; ++i)
        {
            if (pIgnoreList[i] == ls.Severity)
            {
                return true;
            }
        }

        wss log = wss();

        zoned_time lt(current_zone(), time_point_cast<seconds>(ls.Time));
        auto tfmt(format(timeFormat, lt));

        log << L"[" << wstring(tfmt.begin(), tfmt.end()) << L"]"
            << L"[" << GetTag(ls.Severity) << L"]"
            << L" " << ls.Message.c_str() << endl;

        of output(szTargetPath, ios_base::app);
        if (!output.is_open())
        {
            return false;
        }

        output << log.str();
        output.close();

        OutputDebugString(log.str().c_str());
        
        return true;
    }


    const wchar_t* GetTag(const Severity& s)
    {
        switch (s)
        {
            case Bee::Debug::Bee:
                return L"---";

            case Bee::Debug::Info:
                return L"Info";

            case Bee::Debug::Warning:
                return L"Warning";

            case Bee::Debug::Error:
                return L"Error";

            case Bee::Debug::SmartPointers:
                return L"SmartPointers";

            case Bee::Debug::Allocators:
                return L"Allocators";

            case Bee::Debug::DirectX:
                return L"DirectX";

            default:
                return L"???";
        }
    }

private:
    mutex           m_mQueue = {};
    queue<LogStamp> m_StampQueue = {};
    atomic_bool     m_bLoop = true;
    thread          m_tMainLoop = {};
};

Logger& Logger::Get()
{
    return *m_pInstance;
}

Logger::Logger() 
: m_szTargetFile(nullptr),
  m_pImpl(new _Impl()),
  m_pIgnoreList(nullptr),
  m_uIgnoreListSize(0)
{
    m_pImpl->m_tMainLoop = thread(&Logger::Loop, this);

    // Print out the header
    Log(Bee, L"--------------------------------------------------------");
    Log(Bee, L" Bee                                                    ");
    Log(Bee, L"                             Powered by ImBee xd        ");
    Log(Bee, L"--------------------------------------------------------");
}

Logger::~Logger()
{
    // Print out the footer
    Log(Bee, L"--------------------------------------------------------");
    Log(Bee, L"                                          2024          ");
    Log(Bee, L"--------------------------------------------------------");

    // Stop the logging loop
    m_pImpl->m_bLoop.store(false);
    if (m_pImpl->m_tMainLoop.joinable())
    {
        m_pImpl->m_tMainLoop.join();
    }

    while (!m_pImpl->m_StampQueue.empty())
    {
#ifdef _DEBUG
        static int count = 0;

        if (!m_pImpl->ProcessStamp(m_pImpl->m_StampQueue.front(),
                                   m_szTargetFile,
                                   m_uIgnoreListSize,
                                   m_pIgnoreList))
        {
            ++count;
        }

        if (count >= 10) // 10 * WriteTimeoutMs
        {
            break;
        }

        count = 0;
#else
        m_pImpl->ProcessStamp(m_pImpl->m_StampQueue.front(),
                              m_szTargetFile,
                              m_uIgnoreListSize,
                              m_pIgnoreList);
#endif // _DEBUG

        if (!m_pImpl->m_StampQueue.empty())
        {
            m_pImpl->m_StampQueue.pop();
        }
    }
    
    if (m_pIgnoreList)
    {
        delete[] m_pIgnoreList;
    }

    delete m_pImpl;
}

void Logger::SetIgnoredTags(Severity* pSeverityList, const size_t& uArrSize)
{
    m_pIgnoreList = new Severity[uArrSize];
    for (int i = 0; i < uArrSize; ++i)
    {
        m_pIgnoreList[i] = pSeverityList[i];
    }

    m_uIgnoreListSize = uArrSize;
}

void Logger::Log(      Severity&& sev,
                 const wchar_t*   format,
                 ...)
{
          auto     currentTime(system_clock::now());
    const auto&    msgBuffLenght(Debug::LoggerMessageMaxLenght);
          wstring  msgBuff(msgBuffLenght, L'\0');

    va_list args;
    va_start(args, format);
    vswprintf_s(&msgBuff[0],
                msgBuffLenght,
                format,  
                args);
    va_end(args);

    m_pImpl->m_mQueue.lock();
    m_pImpl->m_StampQueue.push({ sev,
                                 move(msgBuff),
                                 move(currentTime) });
    m_pImpl->m_mQueue.unlock();
}

void Logger::SetPath(const wchar_t* szPath)
{
    using of = wofstream;

    const auto& tmpBuffLenght(Debug::MaxPath);
    wchar_t*    tmp(new wchar_t[tmpBuffLenght]);

    wcscpy_s(tmp, tmpBuffLenght, szPath);
    wcscat_s(tmp, tmpBuffLenght, L"Log.txt");

    if (!of(tmp).is_open())
    {
        throw Debug::InvalidArgument(BEE_INVALID_ARGUMENT_MSG L"szPath wasn't a valid path for a file.",
                                     BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    m_szTargetFile = tmp;
}

void Logger::Loop()
{
    static int count = 0;

    while (m_pImpl->m_bLoop.load())
    {
        this_thread::sleep_for(LogTimeOutMS);

        m_pImpl->m_mQueue.lock();
        if (m_pImpl->m_StampQueue.empty() || m_szTargetFile == nullptr)
        {
            m_pImpl->m_mQueue.unlock();
            continue;
        }
        
        auto f(m_pImpl->m_StampQueue.front());
        m_pImpl->m_StampQueue.pop();

        m_pImpl->m_mQueue.unlock();

        if (!m_pImpl->ProcessStamp(f,
                                   m_szTargetFile,
                                   m_uIgnoreListSize,
                                   m_pIgnoreList))
        {
            ++count;
        }
        
        if (count >= 10) // 10 * WriteTimeoutMs
        {
            throw Debug::Exception(L"Logger couldn't write to file after 10 tries.",
                                   BEE_COLLECT_DATA_ON_EXCEPTION());
        }

        count = 0;
    }
}
