#ifdef __linux__

#include "LinuxWindowManager.h"
#include "Window/LinuxWidnow.h"

bool Duckers::Window::Create(int32 iDesktopIndex)
{
    ::Duckers::LinuxWindowsManager& linuxWindowManager = reinterpret_cast<::Duckers::LinuxWindowsManager&>(
                                                                ::Duckers::WindowsManager::Get());

    if (!(m_pDisplay = linuxWindowManager.AskForDisplay(iDesktopIndex))) {
        return false;
    }
   
    m_WindowHandle = XCreateSimpleWindow(m_pDisplay, 
                                         DefaultRootWindow(m_pDisplay),
                                         0, 0,
                                         800, 600, 
                                         1, BlackPixel(m_pDisplay, 0), WhitePixel(m_pDisplay, 0));

    linuxWindowManager.AddWindow(this);
    
    if (!SetWMProtocols()) {
        return false;
    }

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
    cout << "Destroying a window " << this << endl;

    Duckers::WindowsManager::Get().RemoveWindow(this);
}

void Duckers::Window::Update()
{
    XEvent event;
    while (XPending(m_pDisplay) > 0)
    {
        XNextEvent(m_pDisplay, &event);

        switch (event.type)
        {
            case ClientMessage:
                if (static_cast<Atom>(event.xclient.data.l[0]) == m_WMDeleteWindow) {
                    this->Destroy();
                }

                return;
        }

        this->HandleOtherEvents(0);
    }
}

bool Duckers::Window::SetWMProtocols()
{
    m_WMDeleteWindow = XInternAtom(m_pDisplay, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(m_pDisplay, m_WindowHandle, &m_WMDeleteWindow, 1)) {
        return false;
    }

    return true;
}

#endif // !__linux__
