#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

void Application::Initialize()
{
    auto* myWindow = new Bee::App::EmptyWindow();
    myWindow->Initialize();
    myWindow->Show();

    auto myWindow2 = B_CREATE_WIN(
        Bee::App::EmptyWindow, 
        L"TEST", 
        L"TEST", 
        B_CREATE_WIN_DIM(500, 250));

    myWindow2->Initialize();
    myWindow2->Show();
}
