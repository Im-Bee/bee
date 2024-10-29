#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

auto&& myWindowSettings = Bee::App::WindowProperties();
Bee::App::Primitives::EmptyWindow* myWindow = nullptr;


void Application::Initialize()
{
    BEE_LOGGER_SET_SUPPRESION(
        BEE_CREATE_SUPPRESSION_LIST(Bee::Problems::SmartPointers));

    Bee::Problems::CrashHandling::Get().ProtectObject(&m_Renderer);

    if (!B_IS_OKAY(m_Renderer.Initialize()))
        throw Bee::Problems::Exception(L"Failed to initialize Renderer", B_COLLECT_DATA());

    myWindow = new Bee::App::Primitives::EmptyWindow(Bee::Utils::Move(myWindowSettings));
    myWindow->Initialize();
    myWindow->Show();
    myWindow->MoveFrame();
    myWindow->SetDimension(Bee::App::Rectangle(200, 100));
}

void Application::Update()
{
    static auto myRect = Bee::App::Rectangle(0, 0);
    static auto xSign  = 5;
    static auto ySign  = 5;
    myWindow->MoveFrame(myRect);
    myRect.x += xSign;
    myRect.y += ySign;

    if (myRect.x >= 2360)
        xSign = xSign * -1;

    if (myRect.y >= 1340)
        ySign = ySign * -1;

    m_Renderer.Update();
    m_Renderer.Render();
}

void Application::Destroy()
{
    m_Renderer.Destroy();
}


