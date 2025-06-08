#pragma once

#include "ApplicationManager.h"
#include "Window/IWindow.h"


namespace Duckers
{

class DUCKERS_API WindowsManager
{

    friend class Window;
    friend class ApplicationManager;

public:

    virtual ~WindowsManager() 
    { m_pInstance = nullptr; }

    static WindowsManager& Get();

public:

    virtual usize GetWindowsAmount() = 0;

protected:

    virtual void Update()  = 0;

    virtual void AddWindow(IWindow*)  = 0;
   
    virtual void RemoveWindow(IWindow*) = 0;

protected:

    static WindowsManager* m_pInstance;

};

} // !Duckers

