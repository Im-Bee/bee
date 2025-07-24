#pragma once



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
};


} // !Duckers
