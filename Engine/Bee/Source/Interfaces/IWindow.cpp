#include "Bee.hpp"

#include "Interfaces/IWindow.hpp"

// Initialize a variable to a HWDN of a Window Class
#define B_GET_HANDLE(x)                                     \
    auto& x = this->GetHandle();                            \
    if (!x)                                                 \
        throw Problems::CallOnNullptr(BEE_COLLECT_DATA());  \

// Bee::App::EmptyWindow::EmptyWindow(EmptyWindow&& other) noexcept
// {
// }
// 
// Bee::App::EmptyWindow::EmptyWindow(const EmptyWindow& other) noexcept
// {
// }

Bee::Utils::b_success Bee::App::EmptyWindow::Initialize()
{
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
        return B_SUCCESS;
    }
    else
    {
        B_LOG(
            Bee::Problems::Error, 
            L"Couldn't create the window %p, with index %d.", 
            this,
            this->GetIndex());

        return B_BAD;
    }
}

Bee::Utils::b_success Bee::App::EmptyWindow::Show()
{
    B_GET_HANDLE(handle);
    
    if (!ShowWindow(handle, SW_SHOWNORMAL))
    {
        return B_SUCCESS;
    }
    else
    {
        B_LOG(
            Bee::Problems::Error,
            L"Couldn't show the window %p, with index %d.",
            this,
            this->GetIndex());

        return B_FAIL;
    }
}

Bee::Utils::b_success Bee::App::EmptyWindow::Hide()
{
    B_GET_HANDLE(handle);

    if (ShowWindow(handle, SW_HIDE))
    {
        return B_SUCCESS;
    }
    else
    {
        B_LOG(
            Bee::Problems::Error,
            L"Couldn't hide the window %p, with index %d.",
            this,
            this->GetIndex());

        return B_FAIL;
    }
}

Bee::Utils::b_success Bee::App::EmptyWindow::Destroy()
{
    B_GET_HANDLE(handle);

    if (DestroyWindow(handle))
    {
        return B_SUCCESS;
    }
    else
    {
        B_LOG(
            Bee::Problems::Error,
            L"Couldn't destroy the window %p, with index %d.", 
            this, 
            this->GetIndex());

        return B_BAD;
    }
}

void Bee::App::IWindow::SetHandleAndRegister(HWND handle, Bee::App::IWindow* self)
{
    this->SetHandle(handle);
    this->SetIndex(Bee::App::Manager::Get().Register(self));
}

void Bee::App::IWindow::UnRegisterInManager(Bee::App::IWindow* self)
{
    Bee::App::Manager::Get().UnRegister(self);
}
