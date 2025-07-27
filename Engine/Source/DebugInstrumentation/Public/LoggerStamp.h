#pragma once

#include "BString.h"
#include "CoreMinimal.h"



namespace Duckers::Debug
{

enum ESeverity
{
    Info,
    Warning,
    Error
};


struct LoggerStamp
{
    ESeverity Severity;
    const wchar* Message;
};

constexpr inline String ConvertSeverityToString(const ESeverity)
{
    return String();
}


} // !Duckers
