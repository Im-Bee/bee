#pragma once

#include <Shlobj.h>
#include <Knownfolders.h>

namespace Bee::App
{
    struct BEE_API Configuration
    {
        const wchar_t* AppName   = L"unknown_name";
        const wchar_t* ClassName = L"unknown_class";
    };

    class BEE_API Settings
    {
        const Configuration m_Config;

        Settings() : m_Config(LoadConfiguration()) {}

    public:
        ~Settings() = default;

        Settings(Settings&&) = delete;
        Settings(const Settings&) = default;


        static Settings& Get()
        {
            static Settings instance = {};
            return instance;
        }

    public:
        static const Configuration& GetDefaultConfig()
        {
            static const Configuration def = {
                L"default_name",
                L"default_class" };

            return def;
        }

        static const wchar_t* GetDefaultAppdataPath();

    private:
        Configuration LoadConfiguration()
        {
            throw Problems::NotImplemented(BEE_COLLECT_DATA());
        }
    };
}