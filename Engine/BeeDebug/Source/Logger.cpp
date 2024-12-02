#include "BeeDebug.hpp"

#include <atomic>
#include <chrono>
#include <cstdarg>
#include <fstream>
#include <queue>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace Bee::Problems;

typedef std::chrono::system_clock               LoggerClock;
typedef std::chrono::time_point<LoggerClock>    LoggerTimePoint;

constexpr const std::chrono::milliseconds LogTimeOutMS(100);

struct LogStamp
{
    const Severity          Severity;
    wchar_t* Message;
    const LoggerTimePoint   Time;
};

atomic_bool     _bLoop      = true;
thread          _tMainLoop;
queue<LogStamp> _StampQueue = {};

class Bee::Problems::Logger::_Impl
{
public:
    bool ProcessStamp(
        LogStamp& ls, 
        const wchar_t* szTargetPath,
        const size_t& uIgnoreListSize,
        Severity* pIgnoreList)
    {
        using of = wofstream;
        using wss = wstringstream;

        constexpr auto timeFormat = "{0:%H:%M:%S}";

        if (!szTargetPath)
            return false;


        for (size_t i = 0; i < uIgnoreListSize; ++i)
        {
            if (pIgnoreList[i] == ls.Severity)
                return true;
        }

        // if (!m_vSuppressed.empty())
        // {
        //     for (auto& i : m_vSuppressed)
        //     {
        //         if (ls.Severity == i)
        //             return true;
        //     }
        // }

        wss log = wss();

        zoned_time lt(current_zone(), time_point_cast<seconds>(ls.Time));
        auto tfmt = format(timeFormat, lt);
        log << L"[" << wstring(tfmt.begin(), tfmt.end()) << L"]"
            << L"[" << GetTag(ls.Severity) << L"]"
            << L" " << ls.Message << endl;

        of output = of(szTargetPath, ios_base::app);
        if (!output.is_open())
            return false;

        output << log.str();

        output.close();

        OutputDebugString(log.str().c_str());

        free(ls.Message);

        return true;
    }


    const wchar_t* GetTag(const Severity& s)
    {
        switch (s)
        {
            case Bee::Problems::Bee:
                return L"---";

            case Bee::Problems::Info:
                return L"Info";

            case Bee::Problems::Warning:
                return L"Warning";

            case Bee::Problems::Error:
                return L"Error";

            case Bee::Problems::SmartPointers:
                return L"SmartPointers";

            case Bee::Problems::Allocators:
                return L"Allocators";

            case Bee::Problems::DirectX:
                return L"DirectX";

            default:
                return L"???";
        }
    }
};

Logger* Logger::m_pInstance = new Logger();

Logger& Logger::Get()
{
    return *m_pInstance;
}

Logger::Logger() :
    m_szTargetFile(nullptr),
    m_pImpl(new _Impl()),
    m_pIgnoreList(nullptr),
    m_uIgnoreListSize(0)
{
    _tMainLoop = thread(&Logger::Work, this);

    // Print out the header
    this->Log(Bee, L"--------------------------------------------------------");
    this->Log(Bee, L" Bee                                            ");
    this->Log(Bee, L"                             Powered by ImBee   ");
    this->Log(Bee, L"--------------------------------------------------------");
}

Logger::~Logger()
{
    // Print out the footer
    this->Log(Bee, L"--------------------------------------------------------");
    this->Log(Bee, L"                                          2024  ");
    this->Log(Bee, L"--------------------------------------------------------");

    // Stop the logging loop
    _bLoop.store(false);
    if (_tMainLoop.joinable())
        _tMainLoop.join();

    while (!_StampQueue.empty())
    {
#ifdef _DEBUG
        static int count = 0;

        if (!m_pImpl->ProcessStamp(
            _StampQueue.front(),
            m_szTargetFile,
            m_uIgnoreListSize,
            m_pIgnoreList))
        {
            ++count;
        }

        if (count >= 10) // 10 * WriteTimeoutMs
            throw Exception(
                L"Logger couldn't write to file after 10 tries.",
                BEE_COLLECT_DATA());

        count = 0;
#else
        m_pImpl->ProcessStamp(
            _StampQueue.front(),
            m_szTargetFile,
            m_uIgnoreListSize,
            m_pIgnoreList);
#endif // _DEBUG

        if (!_StampQueue.empty())
            _StampQueue.pop();
    }
    
    delete m_pImpl;
}

void Logger::Log(
    Severity&& sev, 
    const wchar_t* format,
    ...)
{
    auto        currentTime   = system_clock::now();
    const auto& msgBuffLenght = Problems::LoggerMessageMaxLenght;
    wchar_t*    msgBuff       = new wchar_t[msgBuffLenght];

    va_list args;
    va_start(args, format);
    vswprintf_s(
        msgBuff, 
        msgBuffLenght,
        format,  
        args);
    va_end(args);

    _StampQueue.push({ 
        sev, 
        msgBuff, 
        move(currentTime) });
}

void Logger::SetPath(const wchar_t* szPath)
{
    using of = wofstream;

    const auto& tmpBuffLenght = Problems::MaxPath;
    wchar_t*    tmp           = new wchar_t[tmpBuffLenght];

    wcscpy_s(tmp, tmpBuffLenght, szPath);
    wcscat_s(tmp, tmpBuffLenght, L"Log.txt");

    if (!of(tmp).is_open())
        throw Problems::InvalidArgument(
            BEE_INVALID_ARGUMENT_MSG L"szPath wasn't a valid path for a file.",
            BEE_COLLECT_DATA());

    m_szTargetFile = tmp;
}

void Logger::Work()
{
    static int count = 0;

    while (_bLoop.load())
    {
        this_thread::sleep_for(LogTimeOutMS);

        if (_StampQueue.empty())
            continue;

        if (!m_pImpl->ProcessStamp(
            _StampQueue.front(),
            m_szTargetFile,
            m_uIgnoreListSize,
            m_pIgnoreList))
        {
            ++count;
        }
        
        if (count >= 10) // 10 * WriteTimeoutMs
            throw Problems::Exception(
                L"Logger couldn't write to file after 10 tries.",
                BEE_COLLECT_DATA());

        count = 0;
        _StampQueue.pop();
    }
}
