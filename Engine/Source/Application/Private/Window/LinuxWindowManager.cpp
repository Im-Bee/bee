#ifdef __linux__

#include "LinuxWindowManager.h"    
#include "Algorithms/Nodes.h"


::Duckers::LinuxWindowsManager::~LinuxWindowsManager() 
{
    // Destroying the window updates head
    while (m_WindowsHead.Data) {
        m_WindowsHead.Data->Destroy();
    }
}


void ::Duckers::LinuxWindowsManager::AddWindow(IWindow* pIWindow)
{
    cout << "Adding window number " << m_WindowsAmount << " with pointer " << pIWindow << " to display " 
         << reinterpret_cast<Window*>(pIWindow)->m_pDisplay << endl;

    ++m_WindowsAmount;

    if (!m_WindowsHead.Data) 
    {
        m_WindowsHead.Data = reinterpret_cast<Window*>(pIWindow);
        m_WindowsHead.pNext = nullptr;
        m_WindowsTail = &m_WindowsHead;

        return;
    }

    m_WindowsTail->pNext = new Node<Window*>(reinterpret_cast<Window*>(pIWindow));
    m_WindowsTail = m_WindowsTail->pNext;
    cout << "Created node " << m_WindowsTail << endl;
    m_WindowsTail->pNext = nullptr;
}



void ::Duckers::LinuxWindowsManager::RemoveWindow(IWindow* pIWindow)
{
    cout << "Removing window from window manager " << pIWindow << endl;

    if (!m_WindowsHead.Data) {
        throw; // TODO
    }

    --m_WindowsAmount;

    if (m_WindowsHead.Data == pIWindow)
    {
        UnmapWindow(m_WindowsHead.Data);
    
        if (!m_WindowsHead.pNext) {
            m_WindowsHead.Data = nullptr;
            m_WindowsHead.pNext = nullptr;
            cout << "No windows left, head is none" << endl;

            return;
        }
        
        m_WindowsHead.Data = m_WindowsHead.pNext->Data;
        m_WindowsHead.pNext = m_WindowsHead.pNext->pNext;
        cout << "New head " << m_WindowsHead.Data << endl;

        return;
    }

    ForEachNode(&m_WindowsHead, [&](Node<Window*>* pNode) { 
        if (!pNode->pNext) {
            return;
        }

        if (pNode->pNext->Data != pIWindow) {
            return;
        }

        auto toBeDeleted = pNode->pNext;
        pNode->pNext = pNode->pNext->pNext;

        UnmapWindow(toBeDeleted->Data);

        // We should delete the node here, but probably, because of current implementation of ForEachNode() we can't.
        // delete toBeDeleted;
        
        cout << "Deleted node " << toBeDeleted << endl;
    });
}



void ::Duckers::LinuxWindowsManager::Update() 
{
    if (!m_WindowsHead.Data) {
        return;
    }

    ForEachNode(&m_WindowsHead, [](Node<Window*>* pNode) { 
        if (!pNode->Data) {
            return;
        }

        pNode->Data->Update();
    });
}


::Display* ::Duckers::LinuxWindowsManager::AskForDisplay(int32)
{
    static ::Display* pDisplay = XOpenDisplay(NULL);

    return pDisplay;
}


void ::Duckers::LinuxWindowsManager::UnmapWindow(Window* pWindow)
{
    XUnmapWindow(pWindow->m_pDisplay, pWindow->m_WindowHandle);
    XFlush(pWindow->m_pDisplay);
    cout << "Unmapped " << pWindow << " for display " << pWindow->m_pDisplay << endl;
}


#endif // !__linux__
