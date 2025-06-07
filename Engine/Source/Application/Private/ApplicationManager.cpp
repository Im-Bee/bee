#include "ApplicationManager.h"
#include "Window/WindowsManager.h"



bool Duckers::ApplicationManager::GetStatus() 
{ return m_bStatus; }


void Duckers::ApplicationManager::Update() 
{
    if (WindowsManager::Get().GetWindowsAmount() == 0) 
    {
        this->m_bStatus = false;
    }

    WindowsManager::Get().Update(); 
}
