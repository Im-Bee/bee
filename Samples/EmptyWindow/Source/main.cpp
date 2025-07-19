#include "Public/Application.h"

EXPORT_APPLICATION(Application);

void Application::Initialize() 
{ 
    m_EmptyWindow.Create();
    m_EmptyWindow.Show();
}
