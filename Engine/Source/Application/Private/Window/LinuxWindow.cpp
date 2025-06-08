#ifdef __linux__

#include "LinuxWindowManager.h"
#include "Window/LinuxWidnow.h"

bool Duckers::Window::Create(int32 /* uDesktopIndex */)
{
    if (!(m_pDisplay = XOpenDisplay(NULL))) {
        return false;
    }
   
    m_WindowHandle = XCreateSimpleWindow(m_pDisplay, 
                                         DefaultRootWindow(m_pDisplay),
                                         0, 0,
                                         800, 600, 
                                         1, BlackPixel(m_pDisplay, 0), WhitePixel(m_pDisplay, 0));

    Duckers::WindowsManager::Get().AddWindow(this);

    return true;
}

bool Duckers::Window::Show() 
{ 
    if (!XMapWindow(m_pDisplay, m_WindowHandle)) {
        return false;
    }
    XFlush(m_pDisplay);

    return true;
}

bool Duckers::Window::Hide() 
{
    if (!XUnmapWindow(m_pDisplay, m_WindowHandle)) {
        return false;
    }
    XFlush(m_pDisplay);

    return true;
}

void Duckers::Window::Destroy() 
{ 
    Duckers::WindowsManager::Get().RemoveWindow(this);

    if (!XCloseDisplay(m_pDisplay)) {
        // TODO: Log
    }
}

void Duckers::Window::Update()
{
    XEvent event;
    while (XPending(m_pDisplay) > 0)
    {
        XNextEvent(m_pDisplay, &event);

        switch (event.type)
        {
        }
    }
}

#endif // !__linux__
