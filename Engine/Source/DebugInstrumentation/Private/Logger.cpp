#include "Logger.h"
#include "LoggerStamp.h"

#include <iostream>


void Duckers::Debug::Logger::LogSingleThreaded(ESeverity severity, const wchar* wszMessageFormat, ...)
{
    wchar wszMessageBuffer[512];

    va_list args;
    va_start(args, wszMessageFormat);
    vswprintf(wszMessageBuffer, 512, wszMessageFormat, args);
    va_end(args);

    std::wcout << L"[" << Debug::ConvertSeverityToConstString(severity) << L"]: " << wszMessageBuffer << std::endl;
}
