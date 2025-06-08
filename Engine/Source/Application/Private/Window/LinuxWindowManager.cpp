#ifdef __linux__

#include "LinuxWindowManager.h"    
#include "Algorithms/Nodes.h"



void ::Duckers::LinuxWindowsManager::AddWindow(IWindow* pWindow)
{
    ++m_WindowsAmount;

    if (!m_WindowsHead.Data) 
    {
        m_WindowsHead.Data = reinterpret_cast<Window*>(pWindow);
        m_WindowsTail = &m_WindowsHead;
        return;
    }

    m_WindowsTail->pNext = new Node<Window*>(reinterpret_cast<Window*>(pWindow));
    m_WindowsTail = m_WindowsTail->pNext;
}


void ::Duckers::LinuxWindowsManager::RemoveWindow(IWindow* pWindow)
{
    if (!m_WindowsHead.Data) {
        throw; // TODO
    }

    --m_WindowsAmount;

    if (!m_WindowsHead.pNext)
    {
        m_WindowsHead.Data = nullptr;
        return;
    }

    auto pW = reinterpret_cast<Window*>(pWindow);
    if (m_WindowsHead.Data == pW) 
    {
        m_WindowsHead.Data = m_WindowsHead.pNext->Data;
        m_WindowsHead.pNext = m_WindowsHead.pNext->pNext;

        delete m_WindowsHead.pNext;
        return;
    }

    ForEachNode(&m_WindowsHead, [&](Node<Window*>* pNode) { 
            if (!pNode->pNext || !pNode->pNext->Data) {
                return;
            }

            if (pNode->pNext->Data != pW) {
                return;
            }

            auto toBeDeleted = pNode->pNext;
            pNode->pNext = pNode->pNext->pNext;

            delete toBeDeleted;
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

#endif // !__linux__