#include "Bee.hpp"

#include "Manager.hpp"

Bee::App::IWindow* Bee::App::Manager::GetMainWindow()
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

    m_Windows.Push(wnd);
    return m_WindowsRollingIndex++;
}

bool Bee::App::Manager::UnRegister(Bee::App::IWindow* wnd)
{
    for (uintmem i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            m_Windows.Pop(i);

            if ((!m_Windows.GetSize()) && 
                (Bee::App::OnClose == Bee::App::CloseOnNoWindow))
                PostQuitMessage(0);

            if ((wnd->GetIndex() == B_WINDOW_MAIN_WINDOW_INDEX) && 
                (Bee::App::OnClose == Bee::App::CloseOnNoMainWindow))
                PostQuitMessage(0);

            return true;
        }
    }

    return false;
}
