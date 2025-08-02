#pragma once

#include "CoreMinimal.h"
#include "IWindow.h"

namespace Duckers
{

class Window : public IWindow
{

public:

    Window() = default;

    virtual ~Window() 
    { this->Destroy(); }

public: 

    virtual void HandleOtherEvents(int32 event) override = 0;

public:

    /**
     * @param uDesktopIndex - A value that represents on what desktop we are creating the window,
     *                        if set to -1, then use the default.
     * */
    bool Create(int32) override final
    { return true; }

    bool Show() override final
    { return true; }

    bool Hide() override final
    { return true; }

    void Destroy() override final
    { }

};

} // !Duckers
