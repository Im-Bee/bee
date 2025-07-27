#include "DetectLoggingMode.h"
#include "Public/Application.h"

EXPORT_APPLICATION(Application);



void Application::Initialize() 
{ 
    D_LOG(D_INFO, L"Test");
}
