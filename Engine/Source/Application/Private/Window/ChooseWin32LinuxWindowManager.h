#pragma once

#include "Window/WindowsManager.h"

#ifdef _WIN32

#   include "Win32WindowManager.h"
    
    ::Duckers::WindowsManager* ::Duckers::WindowsManager::m_pInstance = new ::Duckers::Win32WindowManeger();
    ::Duckers::WindowsManager& ::Duckers::WindowsManager::Get()
    {
        if (!m_pInstance) {
            m_pInstance = new ::Duckers::Win32WindowManeger();
        }
        return *m_pInstance;
    }

#elif __linux__

#   include "LinuxWindowManager.h"

    ::Duckers::WindowsManager* ::Duckers::WindowsManager::m_pInstance = new ::Duckers::LinuxWindowsManager();

    ::Duckers::WindowsManager& ::Duckers::WindowsManager::Get() 
    {   
        if (!m_pInstance) {
            m_pInstance = new ::Duckers::LinuxWindowsManager();
        }
        return *m_pInstance;
    }

#else
#   error "Couldn't detect the system"
#endif // !_WIN32

