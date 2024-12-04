#include "Bee.hpp"

Bee::App::Properties* Bee::App::Properties::m_pInstance = new Bee::App::Properties();

Bee::App::Properties& Bee::App::Properties::Get()
{
    return *m_pInstance;
}

wchar_t* Bee::App::Properties::GetAppdataPath()
{
    const auto& config = Bee::App::Properties::GetDefaultConfig();
    static wchar_t path[BEE_MAX_PATH] = { 0 };

    if (path[0] != 0)
        return path;

#ifdef _DEBUG
    wcscpy_s(path, GetDebuggingDirPath());
    wcscat_s(path, L"Appdata\\");
    B_CREATE_DIR(path);
#else
    PWSTR appdata;

    if (S_OK != SHGetKnownFolderPath(
        FOLDERID_RoamingAppData,
        KF_FLAG_DEFAULT,
        NULL,
        &appdata))
        throw Problems::ProblemWithWINAPI(BEE_COLLECT_DATA());

    wcscpy_s(path, appdata);
    CoTaskMemFree(appdata);
#endif // _DEBUG
    
    wcscat_s(path, L"\\");
    wcscat_s(path, BEE_BEE);
    wcscat_s(path, L"\\");

    B_CREATE_DIR(path);

    wcscat_s(path, config.AppName);
    wcscat_s(path, L"\\");

    B_CREATE_DIR(path);


    return path;
}

wchar_t* Bee::App::Properties::GetResourcesPath()
{
    static wchar_t path[BEE_MAX_PATH] = { 0 };

    if (path[0] != 0)
        return path;

    wcscpy_s(path, GetCurrentPath());
    wcscat_s(path, L"\\Resources\\");
    B_CREATE_DIR(path);

    return path;
}

const wchar_t* Bee::App::Properties::GetCurrentPath()
{
    static wchar_t path[BEE_MAX_PATH] = { 0 };

    if (path[0] != 0)
        return path;

    DWORD e = GetCurrentDirectory(BEE_MAX_PATH, path);
    if (e == 0)
    {
        B_WIN_REPORT_FAILURE();
        wcscpy_s(path, L".\\");
    }

    return path;
}

const wchar_t* Bee::App::Properties::GetDebuggingDirPath()
{
    static wchar_t path[BEE_MAX_PATH] = { 0 };

    if (path[0] != 0)
        return path;

    wcscpy_s(path, GetCurrentPath());
    wcscat_s(path, L"\\DebugDirectories\\");
    B_CREATE_DIR(path);

    return path;
}
