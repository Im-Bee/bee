#pragma once

#include "ApplicationManager.h"
#include "ExportImportEntryApp.h"
#include "IApplication.h"
#include "Logger.h"



int main()
{
    ::Duckers::IAppliciaton& pApp = *::Duckers::ImportApplication();
    auto& pAppManager = ::Duckers::ApplicationManager::Get();
    pApp.Initialize();
    
    while (pAppManager.GetStatus()) 
    {
        pAppManager.Update();

        pApp.Update();
        pApp.Render();
    }

    pApp.Destroy();


    delete &pApp;
    delete &::Duckers::WindowsManager::Get();
    delete &::Duckers::ApplicationManager::Get();

    return 0;
}

