#include "Bee.hpp"

using namespace Bee::Utils;
using namespace Bee::App;

Manager* Manager::m_pInstance = new Manager();

// ----------------------------------------------------------------------------
//                              Public Methods
// ----------------------------------------------------------------------------

Bee::App::Manager::~Manager()
{
    if (!UnregisterClass(B_WINDOW_CLASS, B_HINSTANCE()))
    {
        B_WIN_REPORT_FAILURE();
    }
}

Manager& Manager::Get()
{
    return *m_pInstance;
}

const IWindow* Manager::GetMainWindow() const
{
    for (Memory::uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i]->GetIndex() == B_WINDOW_MAIN_WINDOW_INDEX)
            return m_Windows[i];
    }

    throw Problems::NullptrCall(BEE_COLLECT_DATA());
}

Vec2<int> Bee::App::Manager::GetMonitorResolution() const
{
    return Vec2<int>(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
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
    for (Memory::uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            BEE_LOG(Problems::Warning, L"This window is already registered %p", wnd);
            return wnd->GetIndex();
        }
    }

    BEE_LOG(Problems::Info, L"Register window %p", wnd);
    m_Windows.Push(wnd);
    return m_uWindowsRollingIndex++;
}

b_status Manager::UnRegister(IWindow* wnd)
{
    if (m_bQuit)
        BEE_RETURN_OKAY;

    for (Memory::uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            BEE_LOG(Problems::Info, L"UnRegister window %p", wnd);

            m_Windows[i]->SetHandle(NULL);
            m_Windows.Pop(i);

            if ((!m_Windows.GetSize()) && 
                (OnClose == NoWindow))
            {
                BEE_LOG(
                    Problems::Warning,
                    L"Window %p was the last one, \
 application is shutting down, because (Bee::App::OnClose == Bee::App::CloseAction::NoWindow)",
                    wnd);

                m_bQuit = true;
                Quit();
            }

            if ((wnd->GetIndex() == B_WINDOW_MAIN_WINDOW_INDEX) && 
                (OnClose == NoMainWindow))
            {
                BEE_LOG(
                    Problems::Warning,
                    L"Window %p has an index of B_WINDOW_MAIN_WINDOW_INDEX,\
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
        iter->SetHandle(NULL);
        iter->~IWindow();
        --iter;
    }

    for (Memory::uintmem i = (m_Windows.GetSize() - 1); i != Memory::uintmem(-1); --i)
    {
        // invalidate the window
        // m_Windows[i]->SetHandle(NULL);
        // m_Windows[i]->~IWindow();
        m_Windows.Pop(i);
    }

    PostQuitMessage(0);
}
