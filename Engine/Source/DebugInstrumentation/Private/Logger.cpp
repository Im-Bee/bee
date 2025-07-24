#include "Logger.h"




void Duckers::Debug::Logger::Log()
{
    m_LogsQueue.PushBack(LoggerStamp());
}

