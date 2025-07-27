#include "Nodes.h"
#include "Public/Application.h"
#include "DynamicAllocator.h"
#include "Logger.h"

EXPORT_APPLICATION(Application);



void Application::Initialize() 
{ 
    Duckers::Debug::Logger::Get().LogSingleThreaded(Duckers::Debug::ESeverity::Info, L"Test");
}
