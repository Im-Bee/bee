#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

#include "Bee3D/Include/OpenGL/Primitives/OpenGLWindow.hpp"

Bee::App::IWindow* myWindow = nullptr;

using namespace Bee;
using namespace Bee::Problems;

void Application::Initialize()
{
    BEE_LOGGER_SET_IGNORE_LIST(
        BEE_CREATE_IGNORE_LIST());

    myWindow = new Bee::GL::Primitives::OpenGLWindow();
    myWindow->Initialize();
    myWindow->MoveFrame();
    myWindow->SetDimension(Bee::Rectangle(200, 100));
    myWindow->Show();
}

void Application::Update()
{
}

void Application::Destroy()
{
}
