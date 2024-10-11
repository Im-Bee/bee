#include "BeeDebug.hpp"

#include "Logger.hpp"

#include <cstdarg>
#include <fstream>

using namespace std;
using namespace std::chrono;

Bee::Problems::Logger* Bee::Problems::Logger::m_pInstance = new Bee::Problems::Logger();

Bee::Problems::Logger& Bee::Problems::Logger::Get()
{
    return *m_pInstance;
}

Bee::Problems::Logger::Logger()
    : m_bLoop(true),
    m_tMainLoop(&Bee::Problems::Logger::Work, this)
{
    // Print header
    this->Log(Bee, L"--------------------------------------------------------");
    this->Log(Bee, L" Bee                                            ");
    this->Log(Bee, L"                             Powered by ImBee   ");
    this->Log(Bee, L"--------------------------------------------------------");
}

Bee::Problems::Logger::~Logger()
{
    // Print footer
    this->Log(Bee, L"--------------------------------------------------------");
    this->Log(Bee, L"                                          2024  ");
    this->Log(Bee, L"--------------------------------------------------------");

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
            throw Problems::Exception(
                L"Logger couldn't write to file after 10 tries.",
                B_COLLECT_DATA());

        count = 0;
#else
        ProcessStamp(m_StampQueue.front());
#endif // _DEBUG

        if (!m_StampQueue.empty())
            m_StampQueue.pop();
    }
}

void Bee::Problems::Logger::Log(
    Severity&& sev, 
    const wchar_t* format,
    ...)
{
    auto time = system_clock::now();
    wchar_t* message = new wchar_t[BEE_PROBLEMS_LOGGER_MAX_MESSAGE];

    va_list args;
    va_start(args, format);
    vswprintf_s(
        message, 
        BEE_PROBLEMS_LOGGER_MAX_MESSAGE, 
        format,  
        args);
    va_end(args);

    m_StampQueue.push({ 
        sev, 
        message, 
        move(time) });
}

void Bee::Problems::Logger::SetPath(const wchar_t* szPath)
{
    using of = wofstream;
    wchar_t* tmp = new wchar_t[Problems::MaxPath];

    wcscpy_s(tmp, Problems::MaxPath, szPath);
    wcscat_s(tmp, Problems::MaxPath, L"Log.txt");

    if (!of(tmp).is_open())
        throw Problems::InvalidArgument(
            BEE_INVALID_ARGUMENT_MSG L"szPath wasn't a valid path for a file.",
            B_COLLECT_DATA());

    m_szTargetFile = tmp;
}

void Bee::Problems::Logger::SetSuppressed(SuppressionList&& list)
{
    m_vSuppressed = list;
}

void Bee::Problems::Logger::Work()
{
    static int count = 0;

    while (m_bLoop.load())
    {
        this_thread::sleep_for(WriteTimeoutMs);

        if (m_StampQueue.empty())
            continue;

        if (!ProcessStamp(m_StampQueue.front()))
            ++count;
        
        if (count >= 10) // 10 * WriteTimeoutMs
            throw Problems::Exception(
                L"Logger couldn't write to file after 10 tries.",
                B_COLLECT_DATA());

        count = 0;
        m_StampQueue.pop();
    }
}

bool Bee::Problems::Logger::ProcessStamp(LogStamp& ls)
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

    free(ls.Message);

    return true;
}

const wchar_t* Bee::Problems::Logger::GetTag(const Bee::Problems::Severity& s)
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
        return L"ComPtr";

    case Bee::Problems::Allocators:
        return L"Allocators";

    default:
        return L"???";
    }
}
