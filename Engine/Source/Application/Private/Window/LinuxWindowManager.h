#pragma once

#include "Window/WindowsManager.h"
#include "Window/LinuxWidnow.h"
#include "Memory/Nodes.h"


namespace Duckers
{

class LinuxWindowsManager : public WindowsManager
{

    friend class Window;

public:

    LinuxWindowsManager()
        : WindowsManager()
        , m_WindowsHead(Node<Window*>(nullptr))
    { }

    ~LinuxWindowsManager() = default;
    
public:

    usize GetWindowsAmount() override final 
    { return m_WindowsAmount; }

protected:

    void Update() override;

    void AddWindow(IWindow* pWindow) override;
    
    void RemoveWindow(IWindow* pWindow) override;

private:

    Node<Window*> m_WindowsHead;
    Node<Window*>* m_WindowsTail;
    
    usize m_WindowsAmount = 0;

};

} // !Duckers


