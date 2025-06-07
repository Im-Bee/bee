#pragma once

#include "ApplicationManager.h"
#include "ExportImportEntryApp.h"
#include "IApplication.h"



int main()
{
    ::Duckers::IAppliciaton& pApp = *::Duckers::ImportApplication();
    auto* pAppManager = &::Duckers::ApplicationManager::Get();
    pApp.Initialize();
    
    while (pAppManager->GetStatus()) 
    {
        pAppManager->Update();

        pApp.Update();
        pApp.Render();
    }

    pApp.Destroy();

    ::Duckers::WindowsManager::Get().~WindowsManager();

    return 0;
}

