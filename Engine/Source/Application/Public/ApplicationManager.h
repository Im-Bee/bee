#pragma once

#include "Window/WindowsManager.h"


namespace Duckers
{

class ApplicationManager
{

public:

    ~ApplicationManager() = default;

    static ApplicationManager& Get()
    { 
        static ApplicationManager* pInstance = nullptr;
        if (!pInstance) {
            pInstance = new ApplicationManager();
        }

        return *pInstance;
    }

    ApplicationManager(const ApplicationManager&) = default;
    ApplicationManager(ApplicationManager&&) = delete;

public:

    bool GetStatus();

    void Update();

protected:
    
    ApplicationManager()
        : m_bStatus(true)
    { }

private:

    volatile bool m_bStatus;

};

} // !Duckers

