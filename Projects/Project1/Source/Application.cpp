#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

void Application::Initialize()
{
    Bee::Problems::Logger::Get().SetSuppressed(
        BEE_CREATE_SUPPRESSION_LIST());

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


