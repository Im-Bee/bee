#include "DetectLoggingMode.h"
#include "Public/Application.h"
#include "EmptyWindow.h"

EXPORT_APPLICATION(Application);



void Application::Initialize() 
{ 
    Duckers::EmptyWindow* em = new Duckers::EmptyWindow();
    em->Create();
    em->Show();
}
