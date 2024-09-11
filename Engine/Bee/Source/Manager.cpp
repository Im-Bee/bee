#include "Bee.hpp"

#include "Manager.hpp"

Bee::App::IWindow* Bee::App::Manager::Register(Bee::App::IWindow* wnd)
{
    for (int i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            B_LOG(Problems::Warning, L"This window is already registered %p", wnd);
            return wnd;
        }
    }

    return m_Windows.Push(wnd);
}

bool Bee::App::Manager::UnRegister(Bee::App::IWindow* wnd)
{
    for (int i = 0; i < m_Windows.GetSize(); ++i)
    {
        if (m_Windows[i] == wnd)
        {
            m_Windows.Pop(i);

            if (!m_Windows.GetSize() &&
                Bee::App::CloseOnNoWindow)
                PostQuitMessage(0);

            return true;
        }
    }

    return false;
}
