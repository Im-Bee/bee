#include "Logger.h"
#include "LoggerStamp.h"




void Duckers::Debug::Logger::LogSingleThreaded(ESeverity severity, const wchar* wszMessageFormat)
{
    m_LogsQueue.PushBack(LoggerStamp({ severity, wszMessageFormat }));

    ProcessQueue();
}

void Duckers::Debug::Logger::ProcessQueue()
{
    if (m_LogsQueue.IsEmpty()) {
        return;
    }
    
    LoggerStamp stamp(m_LogsQueue.Pop());
    wcout << L"[" << Debug::ConvertSeverityToConstString(stamp.Severity) << L"]: " << stamp.Message << endl;
}
