#include "Nodes.h"
#ifdef __linux__

#include "Debug.h"
#include "LinuxWindowManager.h"    
#include "Algorithms/Nodes.h"


Duckers::LinuxWindowsManager::~LinuxWindowsManager() 
{
    // Destroying the window updates head
    while (m_pWindowsHead) {
        m_pWindowsHead->Data->Destroy();
    }

    while (m_DisplaysHead.Data) {
        CloseDisplay(m_DisplaysHead.Data);
    }
}


void Duckers::LinuxWindowsManager::AddWindow(IWindow* pIWindow)
{
    D_LOG(D_INFO, L"Adding window number %d with pointer %p to display %p",
          m_WindowsAmount,
          pIWindow,
          reinterpret_cast<Window*>(pIWindow)->m_pDisplay);

    ++m_WindowsAmount;

    if (!m_pWindowsHead) 
    {
        m_pWindowsTail = m_pWindowsHead = new Node<Window*>(dynamic_cast<Window*>(pIWindow));
        D_LOG(D_INFO, L"Created head node %p", m_pWindowsHead);
        return;
    }
    
    auto pNext = m_pWindowsTail->pNext;
    pNext = new Node<Window*>(dynamic_cast<Window*>(pIWindow));
    m_pWindowsTail = pNext;
    D_LOG(D_INFO, L"Created node %p", m_pWindowsTail);
}



void Duckers::LinuxWindowsManager::RemoveWindow(IWindow* pIWindow)
{
    D_LOG(D_INFO, L"Removing window from window manager %p", pIWindow);

    if (!m_pWindowsHead) {
        return;
    }

    --m_WindowsAmount;

    if (m_pWindowsHead->Data == pIWindow)
    {
        UnmapWindow(m_pWindowsHead->Data);
    
        if (!m_pWindowsHead->pNext) {
            delete m_pWindowsHead;
            m_pWindowsHead = nullptr;
            D_LOG(D_INFO, L"No windows left, head is none %p", m_pWindowsHead);

            return;
        }

        auto pTmp = m_pWindowsHead->pNext;
        delete m_pWindowsHead;
        m_pWindowsHead = pTmp;
        
        D_LOG(D_INFO, L"New head %p", m_pWindowsHead->Data);

        return;
    }


    Node<Window*>* pToBeDeleted = nullptr;

    ForEachNode(m_pWindowsHead, [&](Node<Window*>* pNode) { 
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
        D_LOG(D_ERROR, L"Couldn't delete?");
        return;
    }

    delete pToBeDeleted;
    D_LOG(D_INFO, L"Deleted node %p", pToBeDeleted);
}



void Duckers::LinuxWindowsManager::Update() 
{
    if (!m_pWindowsHead) {
        return;
    }

    ForEachNode(m_pWindowsHead, [](Node<Window*>* pNode) { 
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

    D_LOG(D_INFO, L"Unmapped %p for display %p", pWindow, pWindow->m_pDisplay);
}


void ::Duckers::LinuxWindowsManager::CloseDisplay(::Display* pDisplay)
{
    XCloseDisplay(pDisplay);
    m_DisplaysHead.Data = nullptr;

    D_LOG(D_INFO, L"Closed display %p", pDisplay);
}

#endif // !__linux__
