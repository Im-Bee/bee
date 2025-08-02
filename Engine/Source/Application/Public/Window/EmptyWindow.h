#pragma once

#include "Window.h"

namespace Duckers
{

class EmptyWindow final : public Window
{
public:

    EmptyWindow() = default;
    ~EmptyWindow() = default;

public:

    void HandleOtherEvents(int32) override final
    { }

};

} // !Duckers
