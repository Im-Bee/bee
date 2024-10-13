#include "Bee.hpp"

#include "Primitives/EmptyWindow.hpp"

// Bee::App::EmptyWindow::EmptyWindow(EmptyWindow&& other) noexcept
// {
// }
// 
// Bee::App::EmptyWindow::EmptyWindow(const EmptyWindow& other) noexcept
// {
// }

Bee::Utils::b_status Bee::App::Primitives::EmptyWindow::Initialize()
{
    if (this->GetHandle())
    {
        B_LOG(Bee::Problems::Warning, L"Window is already initialized");

        B_RETURN_OKAY;
    }

    WNDCLASSEX wcex = this->GetBaseWndClassEX();

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = m_BaseSettings.Class;

    RegisterClassEx(&wcex);

    this->SetHandle(CreateWindow(
        m_BaseSettings.Class,
        m_BaseSettings.Title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        m_BaseSettings.Dimensions.x,
        m_BaseSettings.Dimensions.y,
        nullptr,
        nullptr,
        B_HINSTANCE(),
        this));

    if (this->GetHandle())
    {
        B_RETURN_SUCCESS;
    }
    else
    {
        B_LOG(
            Bee::Problems::Error,
            L"Couldn't create the window %p, with index %d.",
            this,
            this->GetIndex());

        B_RETURN_BAD;
    }
}
