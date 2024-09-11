#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

void Application::Initialize()
{
    auto* myWindow = new Bee::App::EmptyWindow();
    myWindow->Initialize();
    myWindow->Show();

    auto* myWindow2 = new Bee::App::EmptyWindow();
    myWindow2->Initialize();
    myWindow2->Show();
}
