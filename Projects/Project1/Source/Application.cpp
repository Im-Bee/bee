#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

using namespace Bee;
using namespace Bee::Debug;

void Application::Initialize()
{
    BEE_LOGGER_SET_IGNORED_MSG_LIST(BEE_CREATE_IGNORED_MSG_LIST(::Bee::Debug::SmartPointers));

    if (!BEE_IS_ALREADY_DID(m_Renderer.Initialize()))
    {
        throw ::Bee::Debug::Exception(L"Failed to initialize RendererDX", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
}

void Application::Update()
{
    m_Renderer.Update();
    m_Renderer.Render();

#ifdef _DEBUG
    // ::Bee::App::Manager::Get().CloseApplication();
#endif // _DEBUG
}

void Application::Destroy()
{
    m_Renderer.Destroy();
}
