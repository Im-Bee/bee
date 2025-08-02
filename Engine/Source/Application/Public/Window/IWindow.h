#pragma once

#include "CoreMinimal.h"

namespace Duckers
{

class IWindow
{
public:

    IWindow() = default;

    virtual ~IWindow() = default;

public: 

    virtual void HandleOtherEvents(int32) = 0;

public:

    /** 
     * @param uDesktopIndex - A value that represents on what desktop we are creating the window,
     *                        if set to -1, then use the default.
     * */
    virtual bool Create(int32 iDesktopIndex = -1) = 0;

    virtual bool Show() = 0;
   
    virtual bool Hide() = 0;

    virtual void Destroy() = 0;

};

} // !Duckers
