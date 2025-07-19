#pragma once

#include "Entry/ChooseLinuxWin32.h"
#include "Window/EmptyWindow.h"

class Application : public ::Duckers::IAppliciaton
{

public:

    void Initialize();

    void Update() {}

    void Render() {}

    void Destroy() {}

private:

    Duckers::EmptyWindow m_EmptyWindow;
    
};

