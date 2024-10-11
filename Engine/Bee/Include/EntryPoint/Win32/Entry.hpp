#pragma once

#define BEE_DEFINE_APPLICATION(x) constexpr Bee::App::IApplication* Bee::App::EntryApplication() { return new x(); }

INT WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PSTR lpCmdLine,
    _In_ INT nCmdShow)
{
    // We have to load some bugs, 
    // because it's a buggy application
    BEE_LOAD_PROBLEMS();
    BEE_LOGGER_SET_PATH(Bee::App::Settings::GetDefaultAppdataPath());
    Bee::App::Manager::Get();

    B_LOG(Bee::Problems::Info, L"Initializing application");

    auto& app = *Bee::App::EntryApplication();
    app.Initialize();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        app.Update();
    }

    B_LOG(Bee::Problems::Info, L"Destroying application");

    app.Destroy();

    Bee::App::Manager::Get().~Manager();
    BEE_CLOSE_PROBLEMS();

    return static_cast<int>(msg.wParam);
}