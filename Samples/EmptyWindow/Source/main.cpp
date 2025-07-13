#include "Public/Application.h"
#include "Window/EmptyWindow.h"

EXPORT_APPLICATION(Application);

void Application::Initialize() 
{
    Duckers::EmptyWindow* pWindow = new Duckers::EmptyWindow();
    Duckers::EmptyWindow* pWindow2 = new Duckers::EmptyWindow();

    pWindow->Create();
    pWindow->Show();

    pWindow2->Create();
    pWindow2->Show();
}
