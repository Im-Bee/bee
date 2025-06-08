#pragma once

#include "CoreMinimal.h"

namespace Duckers
{

class DUCKERS_API IWindow
{
public:

    IWindow() = default;

    virtual ~IWindow() 
    { }

public:

    /** 
     * @param uDesktopIndex - A value that represents on what desktop we are creating the window,
     *                        if set to -1, then use the default.
     * */
    virtual bool Create(int32 uDesktopIndex = -1) = 0;

    virtual bool Show() = 0;
   
    virtual bool Hide() = 0;

    virtual void Destroy() = 0;

};

} // !Duckers
