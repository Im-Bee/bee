#pragma once



namespace Duckers::Debug
{

enum ESeverity
{
    Info,
    Warning,
    Error
};


struct DUCKERS_API LoggerStamp
{
    ESeverity Severity;
};


} // !Duckers
