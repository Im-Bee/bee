#pragma once 

#include "CoreMinimal.h"
#include "IWindow.h"


namespace Duckers
{

class DUCKERS_API Window : public IWindow
{
    
    friend class LinuxWindowsManager;

public:

    Window() = default;

    virtual ~Window() 
    { this->Destroy(); }

public: 

    virtual void HandleOtherEvents(int32 event) = 0;

public:

    /** 
     * @param uDesktopIndex - A value that represents on what desktop we are creating the window,
     *                        if set to -1, then use the default.
     * */
    bool Create(int32 iDesktopIndex = -1) override final;

    bool Show() override final;
    
    bool Hide() override final;

    void Destroy() override final;

protected:

    void Update();

private:

    bool SetWMProtocols();

private:

    ::Display* m_pDisplay;
    ::Window m_WindowHandle;
    ::Atom m_WMDeleteWindow;

};

}   // !Duckers
