#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

auto&& myWindowSettings = Bee::App::WindowProperties();
Bee::App::Primitives::EmptyWindow* myWindow = nullptr;


void Application::Initialize()
{
    BEE_LOGGER_SET_IGNORE_LIST(
        BEE_CREATE_IGNORE_LIST(Bee::Problems::SmartPointers));

    if (!B_IS_OKAY(m_Renderer.Initialize()))
        throw Bee::Problems::Exception(L"Failed to initialize Renderer", BEE_COLLECT_DATA());

    myWindow = new Bee::App::Primitives::EmptyWindow(Bee::Utils::Move(myWindowSettings));
    myWindow->Initialize();
    myWindow->Show();
    myWindow->MoveFrame();
    myWindow->SetDimension(Bee::Rectangle(200, 100));
    m_Renderer.~Renderer();
}

void Application::Update()
{
    static auto myRect = Bee::Rectangle(0, 0);
    static auto dRes   = Bee::App::Manager::Get().GetMonitorResolution();
    static auto xSign  = 5;
    static auto ySign  = 5;
    myWindow->MoveFrame(myRect);
    myRect.x += xSign;
    myRect.y += ySign;
    
    if (myRect.x <= 0 ||
        myRect.x >= dRes.x - 200)
        xSign = xSign * -1;
    
    if (myRect.y <= 0 ||
        myRect.y >= dRes.y -100)
        ySign = ySign * -1;
    
    m_Renderer.Update();
    m_Renderer.Render();
}

void Application::Destroy()
{
    m_Renderer.Destroy();

}


