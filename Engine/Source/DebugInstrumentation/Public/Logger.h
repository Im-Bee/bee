#pragma once 

#include "CoreMinimal.h"
#include "BQueue.h"
#include "LoggerStamp.h"

namespace Duckers::Debug
{


class DUCKERS_API Logger
{
    Logger() = default;

    ~Logger() = default;
    
public:

    static Logger& Get()
    {
        static Logger Instance = { };
        return Instance;
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

public:

    void LogSingleThreaded(ESeverity severity, const wchar* wszMessageFormat);

private:

    void ProcessQueue();

private:

    Duckers::Queue<LoggerStamp> m_LogsQueue;

};


} // !Duckers
