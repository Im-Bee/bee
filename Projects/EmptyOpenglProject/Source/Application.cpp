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

    Vec3f a(6.f, -4.f, 5.f);
    Vec3f b(54.f, 3.f, -6.f);
    Vec3f cross(Move(a.CrossProduct(b)));
    BEE_LOG(Info, L"Cross product: %f %f %f", cross.x, cross.y, cross.z);
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
