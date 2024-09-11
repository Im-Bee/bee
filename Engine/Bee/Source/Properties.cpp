#include "Bee.hpp"

#include "Properties.hpp"

const wchar_t* Bee::App::Settings::GetDefaultAppdataPath()
{
    static wchar_t path[B_MAX_PATH] = { 0 };
    if (path[0] != 0)
        return path;

    const auto& config = Bee::App::Settings::GetDefaultConfig();
    PWSTR appdata;

    if (S_OK != SHGetKnownFolderPath(
        FOLDERID_LocalAppData,
        KF_FLAG_DEFAULT,
        NULL,
        &appdata))
        throw Problems::ProblemWithWINAPI(BEE_COLLECT_DATA());

    wcscat_s(path, appdata);
    wcscat_s(path, L"\\");
    wcscat_s(path, B_BEE);
    wcscat_s(path, L"\\");

    B_CREATE_DIR(path);

    wcscat_s(path, config.AppName);
    wcscat_s(path, L"\\");

    B_CREATE_DIR(path);

    CoTaskMemFree(appdata);

    return path;
}