#pragma once 

#include "CoreMinimal.h"
#include "Queue.h"
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

    void Log();

private:

    Duckers::Queue<LoggerStamp> m_LogsQueue;

};


} // !Duckers
