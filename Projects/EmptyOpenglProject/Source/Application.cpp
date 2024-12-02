#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

Bee::App::IWindow* myWindow = nullptr;

using namespace Bee;
using namespace Bee::Problems;

void Application::Initialize()
{
    BEE_LOGGER_SET_IGNORE_LIST(
        BEE_CREATE_IGNORE_LIST());

    m_Renderer.Initialize();
}

void Application::Update()
{
    m_Renderer.Update();
    m_Renderer.Render();
}

void Application::Destroy()
{
    m_Renderer.Destroy();
}
