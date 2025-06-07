#pragma once 

#include "CoreMinimal.h"
#include "IWindow.h"

#include <X11/Xlib.h>



namespace Duckers
{

class DUCKERS_API Window final : public IWindow
{
    
    friend class LinuxWindowsManager;

public:

    Window() = default;

    virtual ~Window() final
    { this->Destroy(); }

public:

    /** 
     * @param uDesktopIndex - A value that represents on what desktop we are creating the window,
     *                        if set to -1, then use the default.
     * */
    bool Create(int32 uDesktopIndex = -1) override final;

    bool Show() override final;
    
    bool Hide() override final;

    void Destroy() override final;

protected:

    void Update();

private:

    ::Display* m_pDisplay;
    ::Window m_WindowHandle;
    
};

}   // !Duckers
