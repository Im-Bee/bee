#include "BeeDebug.hpp"

#include <chrono>
#include <utility>

using namespace std;
using namespace literals::chrono_literals;
using namespace Bee::Debug;

Exception::Exception()
    : m_Collected(ColletedOnException(ExceptionUnknownReason,
                                      ExceptionUnknownFile,
                                      ExceptionLineNotCollected))
{
    OnShutdown();
    PopUp();
}

Exception::Exception(const wchar_t* szReason)
    : m_Collected(ColletedOnException(szReason,
                                      ExceptionUnknownFile,
                                      ExceptionLineNotCollected))
{
    OnShutdown();
    PopUp();
}

Exception::Exception(const wchar_t* szReason, ColletedOnException cd)
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
    
    CrashHandler::Get().HandleObjects();

    BEE_CLOSE_DEBUG();
}

void Exception::PopUp() const
{
    using wss = std::wstringstream;

    wss text = wss();
    text << L"Program encountered an error during it's runtime. \n"
        << L"In file: " << m_Collected.szFile << L" at line: " << m_Collected.Line << L"\n"
        << m_Collected.szWhy;

    MessageBox(NULL,
               text.str().c_str(),
               L"Error",
               MB_OK | MB_ICONEXCLAMATION);
}
