#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

Bee::App::IWindow* myWindow = nullptr;

using namespace Bee;
using namespace Bee::Debug;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;

void Application::Initialize()
{
    BEE_LOGGER_SET_IGNORED_MSG_LIST(BEE_CREATE_IGNORED_MSG_LIST());

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
