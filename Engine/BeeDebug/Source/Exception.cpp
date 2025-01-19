#include "BeeDebug.hpp"

#include <chrono>
#include <utility>

using namespace std;
using namespace Bee::Debug;
using namespace literals::chrono_literals;



#define BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(name, msg)	                        \
    ::Bee::Debug::name::name(ColletedOnException&& cd)		                    \
    : ::Bee::Debug::Exception(msg,												\
                              move(cd))											\
    {}																			\
                                                                                \
    ::Bee::Debug::name::name(								                    \
        const wchar_t* customMsg,												\
        ColletedOnException&& cd)												\
    : ::Bee::Debug::Exception(customMsg,									    \
                              move(cd))											\
    {}



Exception::Exception()
    : m_Collected(ColletedOnException(UnknownReason,
                                      UnknownFile,
                                      LineNotCollected))
{
    OnShutdown();
    PopUp();
}

Exception::Exception(const wchar_t* szReason)
    : m_Collected(ColletedOnException(szReason,
                                      UnknownFile,
                                      LineNotCollected))
{
    OnShutdown();
    PopUp();
}

Exception::Exception(const wchar_t* szReason, ColletedOnException && cd)
    : m_Collected(cd)
{
    m_Collected.szWhy = szReason;
    OnShutdown();
    PopUp();
}

void Exception::OnShutdown() const
{
    BEE_LOG(Error,
            L"Shuting down, because application encountered an unhandled exeception '%ls', in file '%ls', at line '%d'",
            m_Collected.szWhy,
            m_Collected.szFile,
            m_Collected.Line);
    
    CrashHandler::Get().OnException();

    BEE_CLOSE_DEBUG();
}

void Exception::PopUp() const
{
    using wss = std::wstringstream;

    wss text = wss();
    text << L"Program encountered an error during it's runtime. \n"
        << L"In file: " << m_Collected.szFile << L" at line: " << m_Collected.Line << L"\n"
        << m_Collected.szWhy;

    MessageBox(
        NULL,
        text.str().c_str(),
        L"Error",
        MB_OK | MB_ICONEXCLAMATION);
}

BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(NotImplemented,     BEE_NOT_IMPLEMENTED_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(BadAlloc,           BEE_BAD_ALLOC_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(OutsideOfBuffer,    BEE_OUTSIDE_OF_BUFFER_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(InvalidArgument,    BEE_INVALID_ARGUMENT_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(ProblemWithWINAPI,  BEE_PROBLEM_WITH_WIN_API_MSG);
BEE_DEFINE_COLLECT_DATA_CONSTRUCTOR(NullptrCall,        BEE_INVALID_PTR_MSG);
