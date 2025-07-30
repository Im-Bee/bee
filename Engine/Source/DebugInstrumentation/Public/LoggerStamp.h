#pragma once

#include "CoreMinimal.h"



namespace Duckers::Debug
{

enum ESeverity
{
    Info = 1,
    Warning = Info << 1,
    Error = Warning << 1,
};


struct LoggerStamp
{
    ESeverity Severity;
    const wchar* Message;
};

constexpr inline const wchar* ConvertSeverityToConstString(const ESeverity severity)
{
    switch (severity) 
    {
        case Info:
            return L"Info";
        case Warning:
            return L"Warning";
        case Error:
            return L"Error";
        default:
            return L"Unknown";
    }
}


} // !Duckers
