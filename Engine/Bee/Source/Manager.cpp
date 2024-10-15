#include "Bee.hpp"

#include "Manager.hpp"

using namespace Bee::Utils;
using namespace Bee::App;

Manager* Manager::m_pInstance = new Manager();

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

    throw Problems::CallOnNullptr(B_COLLECT_DATA());
}

void Manager::CloseApplication()
{
    PostQuitMessage(0);
}

uint64_t Manager::Register(IWindow* wnd)
{
    for (Memory::uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            B_LOG(Problems::Warning, L"This window is already registered %p", wnd);
            return wnd->GetIndex();
        }
    }

    B_LOG(Problems::Info, L"Register window %p", wnd);
    m_Windows.Push(wnd);
    return m_WindowsRollingIndex++;
}

b_status Manager::UnRegister(IWindow* wnd)
{
    for (Memory::uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            B_LOG(Problems::Info, L"UnRegister window %p", wnd);

            m_Windows.Pop(i);

            if (m_bQuit)
                B_RETURN_SUCCESS;

            if ((!m_Windows.GetSize()) && 
                (OnClose == NoWindow))
            {
                B_LOG(
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
                B_LOG(
                    Problems::Warning,
                    L"Window %p has an index of B_WINDOW_MAIN_WINDOW_INDEX,\
 application is shutting down, because (Bee::App::OnClose == Bee::App::CloseAction::NoMainWindow)", 
                    wnd);

                m_bQuit = true;
                Quit();
            }

            B_RETURN_SUCCESS;
        }
    }

    B_RETURN_FAIL;
}

void Manager::Quit()
{
    // for (Memory::uintmem i = (m_Windows.GetSize() - 1); i != Memory::uintmem(-1); --i)
    // {
    //     m_Windows[i]->~IWindow();
    // }
    
    PostQuitMessage(0);
}
