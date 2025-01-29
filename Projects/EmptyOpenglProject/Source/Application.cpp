#include "Application.hpp"

BEE_DEFINE_APPLICATION(Application);

using namespace Bee;
using namespace Bee::App;
using namespace Bee::Debug;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;

void Application::Initialize()
{
    if (BEE_FAILED(Input::Get().Initialize()))
    {
        throw ::Bee::Debug::Exception(L"Couldn't initialize user input", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
    
    // Burning ship fractal
#if 1
#   define BURNING_SHIP
    m_Raycaster.GetWindow().SwapIndex(&m_GLRenderer.GetWindow());

    m_GLRenderer.LoadMeshFromObj(L"R:\\bee\\Projects\\EmptyOpenglProject\\Resources\\Meshes\\DUCK_0.obj");

    auto camera = GL::Camera::Create();
    m_GLRenderer.SetMainCamera(camera);

    if (BEE_FAILED(m_GLRenderer.Initialize()))
    {
        throw ::Bee::Debug::Exception(L"Couldn't initialize raycaster", BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    Input::Get().AttachAciton(0x57, &::Bee::GL::RendererGL::Aciton,  reinterpret_cast<void*>(&m_GLRenderer));
    Input::Get().AttachAciton(0x41, &::Bee::GL::RendererGL::Aciton2, reinterpret_cast<void*>(&m_GLRenderer));
    Input::Get().AttachAciton(0x44, &::Bee::GL::RendererGL::Aciton3, reinterpret_cast<void*>(&m_GLRenderer));
    Input::Get().AttachAciton(0x53, &::Bee::GL::RendererGL::Aciton4, reinterpret_cast<void*>(&m_GLRenderer));
    Input::Get().AttachAciton(0x58, &::Bee::GL::RendererGL::Aciton5, reinterpret_cast<void*>(&m_GLRenderer));
    Input::Get().AttachAciton(0x5A, &::Bee::GL::RendererGL::Aciton6, reinterpret_cast<void*>(&m_GLRenderer));
    Input::Get().AttachAciton(0x45, &::Bee::GL::RendererGL::Aciton7, reinterpret_cast<void*>(&m_GLRenderer));
    Input::Get().AttachAciton(0x51, &::Bee::GL::RendererGL::Aciton8, reinterpret_cast<void*>(&m_GLRenderer));
#   else
    m_Raycaster.LoadMeshFromObj(L"R:\\bee\\Projects\\EmptyOpenglProject\\Resources\\Meshes\\DUCK_0.obj");
        
    if (BEE_FAILED(m_Raycaster.Initialize()))
    {
        throw ::Bee::Debug::Exception(L"Couldn't initialize raycaster", BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    auto camera = GL::Camera::Create();
    m_Raycaster.SetMainCamera(camera);


    Input::Get().AttachAciton(0x57, &::Bee::GL::RaycasterRenderer::Aciton,  reinterpret_cast<void*>(&m_Raycaster));
    Input::Get().AttachAciton(0x41, &::Bee::GL::RaycasterRenderer::Aciton2, reinterpret_cast<void*>(&m_Raycaster));
    Input::Get().AttachAciton(0x44, &::Bee::GL::RaycasterRenderer::Aciton3, reinterpret_cast<void*>(&m_Raycaster));
    Input::Get().AttachAciton(0x53, &::Bee::GL::RaycasterRenderer::Aciton4, reinterpret_cast<void*>(&m_Raycaster));
    Input::Get().AttachAciton(0x58, &::Bee::GL::RaycasterRenderer::Aciton5, reinterpret_cast<void*>(&m_Raycaster));
    Input::Get().AttachAciton(0x5A, &::Bee::GL::RaycasterRenderer::Aciton6, reinterpret_cast<void*>(&m_Raycaster));
    Input::Get().AttachAciton(0x45, &::Bee::GL::RaycasterRenderer::Aciton7, reinterpret_cast<void*>(&m_Raycaster));
    Input::Get().AttachAciton(0x51, &::Bee::GL::RaycasterRenderer::Aciton8, reinterpret_cast<void*>(&m_Raycaster));



#endif // 1
}

void Application::Update()
{
#ifdef BURNING_SHIP
    m_GLRenderer.Render();
    m_GLRenderer.Update();
#else
    m_Raycaster.Update();
    m_Raycaster.Render();
#endif // BURNING_SHIP
}

void Application::Destroy()
{
    Input::Get().Destroy();
    
#ifdef BURNING_SHIP
    m_GLRenderer.Destroy();
#else
    m_Raycaster.Destroy();
#endif // BURNING_SHIP
}
