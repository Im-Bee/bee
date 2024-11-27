#include "Bee.hpp"

#include "Properties.hpp"

Bee::App::Settings* Bee::App::Settings::m_pInstance = nullptr; // = new Bee::App::Settings();

Bee::App::Settings& Bee::App::Settings::Get()
{
    return *m_pInstance;
}

const wchar_t* Bee::App::Settings::GetDefaultAppdataPath()
{
    const auto& config = Bee::App::Settings::GetDefaultConfig();
    static wchar_t path[B_MAX_PATH] = { 0 };
    if (path[0] != 0)
        return path;

#ifdef _DEBUG
    DWORD e = GetCurrentDirectory(B_MAX_PATH, path);

    if (e == 0)
    {
        B_WIN_REPORT_FAILURE();
        wcscpy_s(path, L".\\");
    }

#else
    PWSTR appdata;

    if (S_OK != SHGetKnownFolderPath(
        FOLDERID_RoamingAppData,
        KF_FLAG_DEFAULT,
        NULL,
        &appdata))
        throw Problems::ProblemWithWINAPI(BEE_COLLECT_DATA());

    wcscat_s(path, appdata);
    CoTaskMemFree(appdata);
#endif // _DEBUG

    wcscat_s(path, L"\\");
    wcscat_s(path, B_BEE);
    wcscat_s(path, L"\\");

    B_CREATE_DIR(path);

    wcscat_s(path, config.AppName);
    wcscat_s(path, L"\\");

    B_CREATE_DIR(path);


    return path;
}