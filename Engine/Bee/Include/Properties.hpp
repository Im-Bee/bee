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

    class BEE_API Properties
    {
        Properties() : m_Config(GetDefaultConfig()) {}

    public:
        ~Properties() = default;

        Properties(Properties&&)      = delete;
        Properties(const Properties&) = default;

        static Properties& Get();

    public:
// Getters --------------------------------------------------------------------
        static const Configuration& GetDefaultConfig()
        {
            static const Configuration def = {
                L"default_name",
                L"default_class" };

            return def;
        }

        const wchar_t* GetAppdataPath();

        const wchar_t* GetResourcesPath();

    private:
        Configuration LoadConfiguration()
        {
            throw Problems::NotImplemented(BEE_COLLECT_DATA());
        }

        const wchar_t* GetCurrentPath();
        const wchar_t* GetDebuggingDirPath();
        
    private:
        const  Configuration m_Config;
        static Properties* m_pInstance;
    };
}