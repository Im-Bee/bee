#pragma once

#include "Window/WindowsManager.h"


namespace Duckers
{

class ApplicationManager
{

public:

    static ApplicationManager& Get()
    { 
        if (!m_pInstance) {
            m_pInstance = new ApplicationManager();
        }
        return *m_pInstance;
    }

    bool GetStatus();

    void Update();

protected:
    
    ~ApplicationManager()
    { m_pInstance = nullptr; }

    ApplicationManager()
        : m_bStatus(true)
    { }

    ApplicationManager(ApplicationManager&&) = default;

    static ApplicationManager* m_pInstance;

private:

    volatile bool m_bStatus;

};

} // !Duckers

