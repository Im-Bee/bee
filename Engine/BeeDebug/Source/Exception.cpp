#include "BeeDebug.hpp"

#include "Exception.hpp"

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN
#	include <Windows.h>
#endif // _WIN32
#include <utility>

using namespace std;
using namespace Bee::Problems;

#define BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(exceptionName, msg)	                \
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

Exception::Exception()
    : m_Collected(CollectedData(
                    NoReason,
                    NoFile,
                    LineNotCollected))
{
    PopUp();
    DumpLogger();
}

Exception::Exception(const wchar_t* szReason)
    : m_Collected(CollectedData(
                    szReason,
                    NoFile,
                    LineNotCollected))
{
    PopUp();
    DumpLogger();
}

Exception::Exception(const wchar_t* szReason, CollectedData && cd)
    : m_Collected(cd)
{
    m_Collected.szWhy = szReason;
    PopUp();
    DumpLogger();
}

void Exception::DumpLogger() const
{
    B_LOG(
        Error,
        L"Throwing, because application %ls, in file %ls, at line %d",
        m_Collected.szWhy,
        m_Collected.szFile,
        m_Collected.Line);

    BEE_CLOSE_PROBLEMS();
}

void Exception::PopUp() const
{
    using wss = std::wstringstream;

    wss text = wss();
    text << L"Program encountered an error during run time. \n"
        << L"In file: " << m_Collected.szFile << L" at line: " << m_Collected.Line
        << L" " << m_Collected.szWhy;

    auto r = MessageBox(
        NULL,
        text.str().c_str(),
        L"Error",
        MB_OK | MB_ICONEXCLAMATION);
}

BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(NotImplemented,     BEE_NOT_IMPLEMENTED_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(OutsideOfBuffer,    BEE_OUTSIDE_OF_BUFFER_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(InvalidArgument,    BEE_INVALID_ARGUMENT_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(ProblemWithWINAPI,  BEE_PROBLEM_WITH_WIN_API_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(CallOnNullptr,      BEE_CALL_ON_NULLPTR_MSG);
