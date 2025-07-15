#include "Nodes.h"
#ifdef __linux__

#include "LinuxWindowManager.h"    
#include "Algorithms/Nodes.h"


Duckers::LinuxWindowsManager::~LinuxWindowsManager() 
{
    // Destroying the window updates head
    while (m_WindowsHead) {
        m_WindowsHead->Data->Destroy();
    }

    while (m_DisplaysHead.Data) {
        CloseDisplay(m_DisplaysHead.Data);
    }
}


void Duckers::LinuxWindowsManager::AddWindow(IWindow* pIWindow)
{
    cout << "Adding window number " << m_WindowsAmount << " with pointer " << pIWindow << " to display " 
         << reinterpret_cast<Window*>(pIWindow)->m_pDisplay << endl;

    ++m_WindowsAmount;

    if (!m_WindowsHead) 
    {
        m_WindowsHead = new Node<Window*>(dynamic_cast<Window*>(pIWindow));
        return;
    }
    
    auto pNext = m_WindowsHead->pNext;
    m_WindowsHead->pNext = new Node<Window*>(dynamic_cast<Window*>(pIWindow));
    m_WindowsHead->pNext->pNext = pNext;
    cout << "Created node " << m_WindowsHead->pNext << endl;
}



void Duckers::LinuxWindowsManager::RemoveWindow(IWindow* pIWindow)
{
    cout << "Removing window from window manager " << pIWindow << endl;

    if (!m_WindowsHead) {
        throw; // TODO
    }

    --m_WindowsAmount;

    if (m_WindowsHead->Data == pIWindow)
    {
        UnmapWindow(m_WindowsHead->Data);
    
        if (!m_WindowsHead->pNext) {
            delete m_WindowsHead;
            m_WindowsHead = nullptr;
            cout << "No windows left, head is none " << m_WindowsHead << endl;

            return;
        }

        auto pTmp = m_WindowsHead->pNext;
        delete m_WindowsHead;
        m_WindowsHead = pTmp;
        
        cout << "New head " << m_WindowsHead->Data << endl;

        return;
    }


    Node<Window*>* pToBeDeleted = nullptr;

    ForEachNode(m_WindowsHead, [&](Node<Window*>* pNode) { 
        if (!pNode->pNext) {
            return;
        }

        if (pNode->pNext->Data != pIWindow) {
            return;
        }

        pToBeDeleted = pNode->pNext;
        pNode->pNext = pNode->pNext->pNext;

        UnmapWindow(pToBeDeleted->Data);
    });

    if (pToBeDeleted) {
        cout << "Couldn't delete?" << endl;
        return;
    }

    delete pToBeDeleted;
    cout << "Deleted node " << pToBeDeleted << endl;
}



void Duckers::LinuxWindowsManager::Update() 
{
    if (!m_WindowsHead) {
        return;
    }

    ForEachNode(m_WindowsHead, [](Node<Window*>* pNode) { 
        if (!pNode->Data) {
            return;
        }

        pNode->Data->Update();
    });
}


::Display* Duckers::LinuxWindowsManager::AskForDisplay(int32)
{
    if (!m_DisplaysHead.Data) {
        m_DisplaysHead.Data = XOpenDisplay(NULL);
    }

    return m_DisplaysHead.Data;
}


void ::Duckers::LinuxWindowsManager::UnmapWindow(Window* pWindow)
{
    XUnmapWindow(pWindow->m_pDisplay, pWindow->m_WindowHandle);
    XFlush(pWindow->m_pDisplay);

    cout << "Unmapped " << pWindow << " for display " << pWindow->m_pDisplay << endl;
}


void ::Duckers::LinuxWindowsManager::CloseDisplay(::Display* pDisplay)
{
    XCloseDisplay(pDisplay);
    m_DisplaysHead.Data = nullptr;

    cout << "Closed display " << pDisplay << endl;
}

#endif // !__linux__
