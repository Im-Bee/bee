#include "BeeDebug.hpp"

#include "Exception.hpp"

#include <utility>

using namespace std;

#define BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(exceptionName, msg)	\
Bee::Problems::exceptionName::exceptionName(CollectedData&& cd)				\
: Bee::Problems::Exception(													\
    msg,																	\
    move(cd))																\
{}																			\
Bee::Problems::exceptionName::exceptionName(								\
    const wchar_t* customMsg,												\
    CollectedData&& cd)														\
: Bee::Problems::Exception(													\
    customMsg,																\
    move(cd))																\
{}

Bee::Problems::Exception::Exception()
    : m_Collected(Problems::CollectedData(
        Problems::NoReason,
        Problems::NoFile,
        Problems::LineNotCollected))
{
    DumpLogger();
}

Bee::Problems::Exception::Exception(const wchar_t* szReason)
    : m_Collected(Problems::CollectedData(
        szReason,
        Problems::NoFile,
        Problems::LineNotCollected))
{
    DumpLogger();
}

Bee::Problems::Exception::Exception(const wchar_t* szReason, CollectedData && cd)
    : m_Collected(cd)
{
    m_Collected.szWhy = szReason;

    DumpLogger();
}

void Bee::Problems::Exception::DumpLogger()
{
    B_LOG(
        Problems::Error,
        L"Throwing, because application %ls, in file %ls, at line %d",
        m_Collected.szWhy,
        m_Collected.szFile,
        m_Collected.Line);

    BEE_CLOSE_PROBLEMS();
}

BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(NotImplemented, BEE_NOT_IMPLEMENTED_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(OutsideOfBuffer, BEE_OUTSIDE_OF_BUFFER_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(InvalidArgument, BEE_INVALID_ARGUMENT_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(ProblemWithWINAPI, BEE_PROBLEM_WITH_WIN_API_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(CallOnNullptr, BEE_CALL_ON_NULLPTR_MSG);
