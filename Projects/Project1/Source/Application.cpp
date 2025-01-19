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

    myWindow = new Bee::App::Primitives::EmptyWindow();
    myWindow->Initialize();
    myWindow->Show();
    myWindow->MoveFrame();
    myWindow->SetDimension(Bee::Rectangle(200, 100));
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
