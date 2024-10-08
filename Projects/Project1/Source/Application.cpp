#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

void Application::Initialize()
{
    Bee::Problems::Logger::Get().SetSuppressed(
        BEE_CREATE_SUPPRESSION_LIST());

    if (!B_IS_SUCCESS(m_Renderer.Initialize()))
        throw Bee::Problems::Exception(L"Failed to initialize Renderer", BEE_COLLECT_DATA());
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


