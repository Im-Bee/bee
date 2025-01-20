#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

using namespace Bee;
using namespace Bee::Debug;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;

void Application::Initialize()
{
    if (BEE_FAILED(m_Raycaster.Initialize()))
    {
        throw ::Bee::Debug::Exception(L"Couldn't initialize raycaster", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
}

void Application::Update()
{
    m_Raycaster.Update();
    m_Raycaster.Render();
}

void Application::Destroy()
{
    m_Raycaster.Destroy();
}
