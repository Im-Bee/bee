#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

Bee::App::Primitives::EmptyWindow* myWindow = nullptr;

using namespace Bee;
using namespace Bee::Problems;

void Application::Initialize()
{
    BEE_LOGGER_SET_IGNORE_LIST(
        BEE_CREATE_IGNORE_LIST());

    myWindow = new Bee::App::Primitives::EmptyWindow();
    myWindow->Initialize();
    myWindow->Show();
    myWindow->MoveFrame();
    myWindow->SetDimension(Bee::Rectangle(200, 100));
}

void Application::Update()
{
}

void Application::Destroy()
{
}
