#pragma once

#include "CoreMinimal.h"
#include "IWindow.h"

namespace Duckers
{

class DUCKERS_API Window final : public IWindow
{

public:

    Window() = default;

    virtual ~Window() final
    { this->Destroy(); }

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
