#include "Public/Application.h"

EXPORT_APPLICATION(Application);

void Application::Initialize() 
{ 
    m_EmptyWindow.Create(0);
    m_EmptyWindow.Show();
}
