#include "ApplicationManager.h"
#include "Window/WindowsManager.h"


::Duckers::ApplicationManager* ::Duckers::ApplicationManager::m_pInstance = new ::Duckers::ApplicationManager();


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
