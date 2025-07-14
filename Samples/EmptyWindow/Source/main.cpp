#include "Public/Application.h"
#include "Window/EmptyWindow.h"
#include "Memory/GrowingAllocator.h"
#include "Algorithms/Arrays.h"

EXPORT_APPLICATION(Application);

void Application::Initialize() 
{
    Duckers::EmptyWindow* pWindow = new Duckers::EmptyWindow();
    Duckers::EmptyWindow* pWindow2 = new Duckers::EmptyWindow();

    pWindow->Create();
    pWindow->Show();

    pWindow2->Create();
    pWindow2->Show();

    Duckers::GrowingAllocator<int> growAlloc = { };

    int pArray[5] = { 0, 1, 2, 3, 4 };
    Duckers::ForEach(pArray, 5, [](auto item) { cout << item << endl; });

}
