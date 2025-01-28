#include "Bee.hpp"

#include "../../BeeUtil/Include/SmartPointers/SharedPtr.hpp"

using namespace Bee::App;
using namespace Bee::Utils;

FileData::~FileData()
{
    if (Buffer)
    {
        delete[] Buffer;
    }
}

Manager* Manager::m_pInstance = new Manager();

// ----------------------------------------------------------------------------
//                              Public Methods
// ----------------------------------------------------------------------------

Manager::~Manager()
{
    if (!::UnregisterClass(BEE_WINDOW_CLASS, B_HINSTANCE()))
    {
        B_WIN_REPORT_FAILURE();
    }
}

Manager& Manager::Get()
{
    return *m_pInstance;
}

FileData Manager::ReadFile(const wchar_t* wszPath)
{
    LARGE_INTEGER fileSize;
    OFSTRUCT      ofstrct = {};
    HANDLE        hFile   = ::CreateFile(wszPath,
                                         GENERIC_READ,
                                         FILE_SHARE_READ,
                                         NULL,
                                         OPEN_EXISTING,
                                         FILE_ATTRIBUTE_NORMAL,
                                         NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        B_WIN_REPORT_FAILURE();
        return FileData(nullptr, 0);
    }

    if (!::GetFileSizeEx(hFile, &fileSize))
    {
        B_WIN_REPORT_FAILURE();
        return FileData(nullptr, 0);
    }

    FileData result(new char[fileSize.QuadPart](), fileSize.QuadPart);

    if (!::ReadFile(hFile,
                    const_cast<char*>(result.Buffer),
                    static_cast<DWORD>(result.Size),
                    NULL,
                    NULL))
    {
        B_WIN_REPORT_FAILURE();
        return FileData(nullptr, 0);
    }

    const_cast<char*>(result.Buffer)[result.Size - 1] = '\0';

    if (!::CloseHandle(hFile))
    {
        B_WIN_REPORT_FAILURE();
    }

    return result;
}

const IWindow* Manager::GetMainWindow() const
{
    for (Usize i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i]->GetIndex() == BEE_WINDOW_MAIN_WINDOW_INDEX)
        {
            return m_Windows[i];
        }
    }

    throw ::Bee::Debug::NullptrCall(BEE_COLLECT_DATA_ON_EXCEPTION());
}

Vec2<int32_t> Manager::GetMonitorResolution() const
{
    return Vec2<int32_t>(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
}

void Manager::CloseApplication()
{
    PostQuitMessage(0);
}

// ----------------------------------------------------------------------------
//                              Private Methods
// ----------------------------------------------------------------------------

uint64_t Manager::Register(IWindow* wnd)
{
    for (Memory::b_usize i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            BEE_LOG(::Bee::Debug::Warning, L"This window is already registered %p", wnd);
            return wnd->GetIndex();
        }
    }

    BEE_LOG(Debug::Info, L"Register window %p", wnd);
    m_Windows.Push(wnd);
    return m_uWindowsRollingIndex++;
}

b_status Manager::UnRegister(IWindow* wnd)
{
    if (m_bQuit)
    {
        BEE_RETURN_OKAY;
    }

    for (Memory::b_usize i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            BEE_LOG(Debug::Info, L"UnRegister window %p", wnd);

            m_Windows[i]->SetHandle(NULL);
            m_Windows.Pop(i);

            if ((!m_Windows.GetSize()) && 
                (OnClose == NoWindow))
            {
                BEE_LOG(
                    Debug::Warning,
                    L"Window %p was the last one, \
 application is shutting down, because (Bee::App::OnClose == Bee::App::CloseAction::NoWindow)",
                    wnd);

                m_bQuit = true;
                Quit();
            }

            if ((wnd->GetIndex() == BEE_WINDOW_MAIN_WINDOW_INDEX) && 
                (OnClose == NoMainWindow))
            {
                BEE_LOG(
                    Debug::Warning,
                    L"Window %p has an index of BEE_WINDOW_MAIN_WINDOW_INDEX,\
 application is shutting down, because (Bee::App::OnClose == Bee::App::CloseAction::NoMainWindow)", 
                    wnd);

                m_bQuit = true;
                Quit();
            }

            BEE_RETURN_SUCCESS;
        }
    }

    BEE_RETURN_FAIL;
}

void Manager::Quit()
{
    auto iter = m_Windows.GetEnd();
    while (iter != m_Windows.GetBegin())
    {
        iter.Ref()->SetHandle(NULL);
        --iter;
    }

    for (Usize i = (m_Windows.GetSize() - 1); i != Usize(-1); --i)
    {
        // invalidate the window
        // m_Windows[i]->SetHandle(NULL);
        // m_Windows[i]->~IWindow();
        m_Windows.Pop(i);
    }

    PostQuitMessage(0);
}

