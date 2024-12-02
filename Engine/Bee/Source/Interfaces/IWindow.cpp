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

Bee::App::IWindow::~IWindow()
{
    Destroy();
}

Bee::Rectangle Bee::App::IWindow::GetCurrentDimensions() const
{
    RECT winRect;
    GetWindowRect(GetHandle(), &winRect);
    return Rectangle(winRect.right - winRect.left, winRect.bottom - winRect.top);
}

Bee::Rectangle Bee::App::IWindow::GetCurrentPos() const
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
    auto dim = GetCurrentDimensions();

    if(!MoveWindow(GetHandle(),
                   rPos.x,
                   rPos.y,
                   dim.x,
                   dim.y,
                   FALSE))
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Problems::Error, L"IWindow (%p): Couldn't move the winodw", this);
    }
}

void Bee::App::IWindow::SetDimension(const Rectangle& rDim)
{
    auto pos = GetCurrentPos();

    if (!MoveWindow(GetHandle(),
                    pos.x,
                    pos.y,
                    rDim.x,
                    rDim.y,
                    FALSE))
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Problems::Error, L"IWindow (%p): Couldn't move the winodw", this);
    }
}

b_status Bee::App::IWindow::Initialize()
{
    if (this->GetHandle())
    {
        BEE_LOG(Problems::Warning, L"Window is already initialized");

        BEE_RETURN_OKAY;
    }

    WNDCLASSEX wcex = this->GetWndClassEX();

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
        BEE_RETURN_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(
            Problems::Error,
            L"Couldn't create the window %p, with index %d.",
            this,
            this->GetIndex());

        BEE_RETURN_BAD;
    }
}

b_status IWindow::Show()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (!ShowWindow(handle, SW_SHOWNORMAL))
    {
        BEE_RETURN_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(
            Problems::Error,
            L"Couldn't show the window %p, with index %d.",
            this,
            this->GetIndex());

        BEE_RETURN_FAIL;
    }
}

b_status IWindow::Hide()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (ShowWindow(handle, SW_HIDE))
    {
        BEE_RETURN_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(
            Problems::Error,
            L"Couldn't hide the window %p, with index %d.",
            this,
            this->GetIndex());

        BEE_RETURN_FAIL;
    }
}

b_status IWindow::Destroy()
{
    if (!m_Handle)
        BEE_RETURN_OKAY;

    if (DestroyWindow(m_Handle))
    {
        m_Handle = NULL;
        BEE_RETURN_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(
            Problems::Error,
            L"Couldn't destroy the window %p, with index %d.",
            this,
            this->GetIndex());

        BEE_RETURN_BAD;
    }
}

void IWindow::RegisterInManager()
{
    SetIndex(Manager::Get().Register(this));
}

void IWindow::UnRegisterInManager()
{
    if (BEE_FAILED(Manager::Get().UnRegister(this)))
        throw Problems::Exception(L"A IWindow couldn't UnRegister itself", BEE_COLLECT_DATA());
}
