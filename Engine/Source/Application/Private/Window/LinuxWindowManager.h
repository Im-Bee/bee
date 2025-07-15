#pragma once

#include "Window/WindowsManager.h"
#include "Window/LinuxWidnow.h"
#include "Memory/Nodes.h"


namespace Duckers
{

class DUCKERS_API LinuxWindowsManager : public WindowsManager
{

    friend class Window;

public:

    LinuxWindowsManager()
        : WindowsManager()
        , m_WindowsHead(nullptr)
    { }

    ~LinuxWindowsManager();
    
public:

    usize GetWindowsAmount() override final 
    { return m_WindowsAmount; }

protected:

    void Update() override;

    void AddWindow(IWindow* pIWindow) override;
    
    void RemoveWindow(IWindow* pIWindow) override;

    ::Display* AskForDisplay(int32 iDisplayIndex);

private:

    void UnmapWindow(Window* pWindow);

    void CloseDisplay(::Display* pDisplay);

private:

    Node<Window*>* m_WindowsHead;
    
    usize m_WindowsAmount = 0;

    Node<::Display*> m_DisplaysHead;

};

} // !Duckers


