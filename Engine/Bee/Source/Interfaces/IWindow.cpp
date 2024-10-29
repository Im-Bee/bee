#include "Bee.hpp"

#include "Interfaces/IWindow.hpp"

using namespace Bee::Utils;
using namespace Bee::App;

Bee::App::IWindow::IWindow()
    : m_BaseSettings(WindowProperties())
{
    RegisterInManager();
}

Bee::App::IWindow::IWindow(WindowProperties baseSettings)
    : m_BaseSettings(baseSettings)
{
    RegisterInManager();
}

Bee::App::IWindow::IWindow(WindowProperties&& baseSettings)
    : m_BaseSettings(Move(baseSettings))
{
    RegisterInManager();
}

Bee::App::IWindow::~IWindow()
{
    if (m_Handle != NULL)
        Destroy();
}

Bee::App::Rectangle Bee::App::IWindow::GetCurrentDimensions() const
{
    RECT winRect;
    GetWindowRect(GetHandle(), &winRect);
    return Rectangle(winRect.right - winRect.left, winRect.bottom - winRect.top);
}

Bee::App::Rectangle Bee::App::IWindow::GetCurrentPos() const
{
    RECT winRect;
    GetWindowRect(GetHandle(), &winRect);
    return Rectangle(winRect.right, winRect.bottom);
}

void Bee::App::IWindow::SwapIndex(Bee::App::IWindow* other)
{
    auto tmp = other->GetIndex();
    other->m_Index = this->m_Index;
    this->m_Index = tmp;
}

void Bee::App::IWindow::MoveFrame(const Rectangle& rPos)
{
    auto&& dim = GetCurrentDimensions();

    if(!MoveWindow(GetHandle(),
                   rPos.x,
                   rPos.y,
                   dim.x,
                   dim.y,
                   FALSE))
    {
        B_WIN_REPORT_FAILURE();
        B_LOG(Problems::Error, L"IWindow (%p): Couldn't move the winodw", this);
    }
}

void Bee::App::IWindow::SetDimension(const Rectangle& rDim)
{
    auto&& pos = GetCurrentPos();

    if (!MoveWindow(GetHandle(),
                    pos.x,
                    pos.y,
                    rDim.x,
                    rDim.y,
                    FALSE))
    {
        B_WIN_REPORT_FAILURE();
        B_LOG(Problems::Error, L"IWindow (%p): Couldn't move the winodw", this);
    }
}

b_status IWindow::Show()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (!ShowWindow(handle, SW_SHOWNORMAL))
    {
        B_RETURN_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        B_LOG(
            Problems::Error,
            L"Couldn't show the window %p, with index %d.",
            this,
            this->GetIndex());

        B_RETURN_FAIL;
    }
}

b_status IWindow::Hide()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (ShowWindow(handle, SW_HIDE))
    {
        B_RETURN_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        B_LOG(
            Problems::Error,
            L"Couldn't hide the window %p, with index %d.",
            this,
            this->GetIndex());

        B_RETURN_FAIL;
    }
}

b_status IWindow::Destroy()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (DestroyWindow(handle))
    {
        B_RETURN_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        B_LOG(
            Problems::Error,
            L"Couldn't destroy the window %p, with index %d.",
            this,
            this->GetIndex());

        B_RETURN_BAD;
    }
}

void IWindow::RegisterInManager()
{
    SetIndex(Manager::Get().Register(this));
}

void IWindow::UnRegisterInManager()
{
    if (B_IS_FAIL(Manager::Get().UnRegister(this)))
        throw Problems::Exception(L"A IWindow couldn't UnRegister itself", B_COLLECT_DATA());
}
