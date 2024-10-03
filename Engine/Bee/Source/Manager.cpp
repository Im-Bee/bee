#include "Bee.hpp"

#include "Manager.hpp"

Bee::App::Manager* Bee::App::Manager::m_pInstance = new Bee::App::Manager();

Bee::App::Manager& Bee::App::Manager::Get()
{
    return *m_pInstance;
}

const Bee::App::IWindow* Bee::App::Manager::GetMainWindow()
{
    for (uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i]->GetIndex() == B_WINDOW_MAIN_WINDOW_INDEX)
            return m_Windows[i];
    }

    throw Bee::Problems::CallOnNullptr(BEE_COLLECT_DATA());
}

void Bee::App::Manager::CloseApplication()
{
    PostQuitMessage(0);
}

uint64_t Bee::App::Manager::Register(Bee::App::IWindow* wnd)
{
    for (uintmem i = 0; i < m_Windows.GetSize(); ++i)
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

Bee::Utils::b_success Bee::App::Manager::UnRegister(Bee::App::IWindow* wnd)
{
    for (uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        B_LOG(Problems::Info, L"UnRegister window %p", wnd);

        if (m_Windows[i] == wnd)
        {
            m_Windows.Pop(i);

            if ((!m_Windows.GetSize()) && 
                (Bee::App::OnClose == Bee::App::CloseOnNoWindow))
                PostQuitMessage(0);

            if ((wnd->GetIndex() == B_WINDOW_MAIN_WINDOW_INDEX) && 
                (Bee::App::OnClose == Bee::App::CloseOnNoMainWindow))
            {
                B_LOG(
                    Problems::Warning,
                    L"Window %p has an index of B_WINDOW_MAIN_WINDOW_INDEX,\
 application is shutting down, because (Bee::App::OnClose == Bee::App::CloseOnNoMainWindow)", 
                    wnd);
                PostQuitMessage(0);
            }

            B_RETURN_SUCCESS;
        }
    }

    B_RETURN_FAIL;
}
