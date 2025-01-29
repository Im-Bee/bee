#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

Bee::App::IWindow* myWindow = nullptr;

using namespace Bee;
using namespace Bee::Debug;

void Application::Initialize()
{
    BEE_LOGGER_SET_IGNORED_MSG_LIST(BEE_CREATE_IGNORED_MSG_LIST());

    if (!BEE_IS_OKAY(m_Renderer.Initialize()))
    {
        throw Bee::Debug::Exception(L"Failed to initialize RendererDX", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
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
