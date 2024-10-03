#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

void Application::Initialize()
{
    // Bee::Problems::Logger::Get().SetSuppressed(
    //     {
    //         Bee::Problems::Info
    //     });

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

    char t = 0xFF;
    B_LOG(Bee::Problems::Info, L"Test = %d", t);


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


