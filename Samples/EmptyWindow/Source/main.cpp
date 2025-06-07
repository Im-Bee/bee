#include "Public/Application.h"
#include "Window/Window.h"

EXPORT_APPLICATION(Application);

void Application::Initialize() 
{
    auto window = new Duckers::Window();

    if (!window->Create()) {
        throw;
    }
    if (!window->Show()) {
        throw;
    }
    if (!window->Hide()) {
        throw;
    }
}
