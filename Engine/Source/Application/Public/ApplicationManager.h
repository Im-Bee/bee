#pragma once

#include "Window/WindowsManager.h"


namespace Duckers
{

class ApplicationManager
{

public:

    static ApplicationManager& Get()
    { 
        static ApplicationManager Instance = {};
        return Instance;
    }

    bool GetStatus();

    void Update();

protected:
    
    ApplicationManager()
        : m_bStatus(true)
    { }

    ~ApplicationManager() = default;

    ApplicationManager(ApplicationManager&&) = default;

private:

    volatile bool m_bStatus;

};

} // !Duckers

