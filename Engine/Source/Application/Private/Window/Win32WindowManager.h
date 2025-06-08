#pragma once

#include "Window/WindowsManager.h"
#include "Memory/Nodes.h"


namespace Duckers
{

class DUCKERS_API Win32WindowManeger : public WindowsManager
{

    friend class Window;

public:

    Win32WindowManeger()
        : WindowsManager()
        , m_WindowsHead(Node<Window*>(nullptr))
    {
    }

    ~Win32WindowManeger() = default;

public:

    usize GetWindowsAmount() override final
    {
        return m_WindowsAmount;
    }

protected:

    void Update() override
    { }

    void AddWindow(IWindow*) override
    { }

    void RemoveWindow(IWindow*) override
    { }

private:

    Node<Window*> m_WindowsHead;
    Node<Window*>* m_WindowsTail;

    usize m_WindowsAmount = 0;

};

} // !Duckers