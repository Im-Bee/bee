#include "BeeDebug.hpp"

#include <cstdarg>
#include <fstream>

#include "Logger.hpp"

using namespace std;
using namespace std::chrono;
using namespace Bee::Problems;

Logger* Logger::m_pInstance = new Logger();

Logger& Logger::Get()
{
    return *m_pInstance;
}

Logger::Logger() :
    m_szTargetFile(nullptr),
    m_vSuppressed(),
    m_bLoop(true),
    m_StampQueue(),
    m_tMainLoop(&Logger::Work, this)
{
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
    m_bLoop.store(false);
    if (m_tMainLoop.joinable())
        m_tMainLoop.join();

    while (!m_StampQueue.empty())
    {
#ifdef _DEBUG
        static int count = 0;
        if (!ProcessStamp(m_StampQueue.front()))
            ++count;

        if (count >= 10) // 10 * WriteTimeoutMs
            throw Exception(
                L"Logger couldn't write to file after 10 tries.",
                BEE_COLLECT_DATA());

        count = 0;
#else
        ProcessStamp(m_StampQueue.front());
#endif // _DEBUG

        if (!m_StampQueue.empty())
            m_StampQueue.pop();
    }
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

    m_StampQueue.push({ 
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

void Logger::SetIgnore(IgnoreList&& list)
{
    m_vSuppressed = list;
}

void Logger::Work()
{
    static int count = 0;

    while (m_bLoop.load())
    {
        this_thread::sleep_for(LogTimeOutMS);

        if (m_StampQueue.empty())
            continue;

        if (!ProcessStamp(m_StampQueue.front()))
            ++count;
        
        if (count >= 10) // 10 * WriteTimeoutMs
            throw Problems::Exception(
                L"Logger couldn't write to file after 10 tries.",
                BEE_COLLECT_DATA());

        count = 0;
        m_StampQueue.pop();
    }
}

bool Logger::ProcessStamp(LogStamp& ls)
{
    using of  = wofstream;
    using wss = wstringstream;

    constexpr auto timeFormat = "{0:%H:%M:%S}";

    if (!m_szTargetFile)
        return false;

    if (!m_vSuppressed.empty())
    {
        for (auto& i : m_vSuppressed)
        {
            if (ls.Severity == i)
                return true;
        }
    }

    wss log = wss();

    zoned_time lt(current_zone(), time_point_cast<seconds>(ls.Time));
    auto tfmt = format(timeFormat, lt);
    log << L"[" << wstring(tfmt.begin(), tfmt.end()) << L"]"
        << L"[" << GetTag(ls.Severity) << L"]"
        << L" " << ls.Message << endl;

    of output = of(m_szTargetFile, ios_base::app);
    if (!output.is_open())
        return false;

    output << log.str();

    output.close();

    OutputDebugString(log.str().c_str());

    free(ls.Message);

    return true;
}

const wchar_t* Logger::GetTag(const Severity& s)
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
