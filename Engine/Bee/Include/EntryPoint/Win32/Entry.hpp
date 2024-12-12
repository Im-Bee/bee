#pragma once

#define BEE_DEFINE_APPLICATION(x) constexpr Bee::App::IApplication* Bee::App::EntryApplication() { return new x(); }

INT WINAPI WinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     PSTR      lpCmdLine,
    _In_     INT       nCmdShow)
{
    // We have to load some bugs, 
    // because it's a buggy application
    BEE_LOAD_DEBUG();
    BEE_LOGGER_SET_OUT_PATH(Bee::App::Properties::Get().GetAppdataPath());
    Bee::App::Manager::Get();
    BEE_LOG(Bee::Problems::Info, L"Initializing application");

    auto& app = *Bee::App::EntryApplication();
    app.Initialize();

    MSG msg = { 0 };

    while(msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            app.Update();
        }
    }

    BEE_LOG(Bee::Problems::Info, L"Destroying application");

    app.Destroy();

    Bee::App::Manager::Get().~Manager();
    BEE_CLOSE_DEBUG();
    return static_cast<INT>(msg.wParam);
}