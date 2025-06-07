#include "Bee.hpp"

using namespace Bee::App;
using namespace Bee::Utils;

IWindow::IWindow()
    : m_BaseSettings(WindowProperties())
{
    RegisterInManager();
}

IWindow::IWindow(WindowProperties baseSettings)
    : m_BaseSettings(baseSettings)
{
    RegisterInManager();
}

IWindow::~IWindow()
{
    Destroy();
}

::Bee::Utils::Rectangle IWindow::GetCurrentDimensions() const
{
    RECT winRect;
    GetWindowRect(GetHandle(), &winRect);
    return Rectangle(static_cast<float>(winRect.right - winRect.left), static_cast<float>(winRect.bottom - winRect.top));
}

::Bee::Utils::Rectangle IWindow::GetCurrentPos() const
{
    RECT winRect;
    GetWindowRect(GetHandle(), &winRect);
    return Rectangle(static_cast<float>(winRect.right), static_cast<float>(winRect.bottom));
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
    HWND handle = GetHandle();
    if (!handle)
    {
        BEE_LOG(Debug::Warning, L"IWindow (%p): Couldn't move the winodw, because of NULL handle", this);
        return;
    }

    if(!MoveWindow(handle,
                   static_cast<int>(rPos.x),
                   static_cast<int>(rPos.y),
                   static_cast<int>(dim.x),
                   static_cast<int>(dim.y),
                   FALSE))
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Debug::Error, L"IWindow (%p): Couldn't move the winodw", this);
    }
}

void Bee::App::IWindow::SetDimension(const Rectangle& rDim)
{
    auto pos = GetCurrentPos();
    HWND handle = GetHandle();
    if (!handle)
    {
        BEE_LOG(Debug::Warning, L"IWindow (%p): Couldn't move the winodw, because of NULL handle", this);
        return;
    }

    if (!MoveWindow(handle,
                    static_cast<int>(pos.x),
                    static_cast<int>(pos.y),
                    static_cast<int>(rDim.x),
                    static_cast<int>(rDim.y),
                    FALSE))
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Debug::Error, L"IWindow (%p): Couldn't move the winodw", this);
    }
}

b_status Bee::App::IWindow::Initialize()
{
    if (this->GetHandle())
    {
        BEE_LOG(Debug::Warning, L"Window is already initialized");

        return BEE_NOTHING_TO_DO;
    }

    WNDCLASSEX wcex = this->GetWndClassEX();

    RegisterClassEx(&wcex);

    this->SetHandle(CreateWindow(BEE_WINDOW_CLASS,
                                 m_BaseSettings.Title,
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 static_cast<int>(m_BaseSettings.Dimensions.x),
                                 static_cast<int>(m_BaseSettings.Dimensions.y),
                                 nullptr,
                                 nullptr,
                                 B_HINSTANCE(),
                                 this));

    if (this->GetHandle())
    {
        return BEE_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Debug::Error,
                L"Couldn't create the window %p, with index %d.",
                this,
                this->GetIndex());

        return BEE_CORRUPTION;
    }
}

b_status IWindow::Show()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (!ShowWindow(handle, SW_SHOWNORMAL))
    {
        return BEE_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Debug::Warning,
                L"Couldn't show the window %p, with index %d. Possible that the window is already shown.",
                this,
                this->GetIndex());

        return BEE_FAIL;
    }
}

b_status IWindow::Hide()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (ShowWindow(handle, SW_HIDE))
    {
        return BEE_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Debug::Warning,
                L"Couldn't hide the window %p, with index %d. Possible that the window is already hiden.",
                this,
                this->GetIndex());

        return BEE_FAIL;
    }
}

b_status IWindow::Destroy()
{
    if (!m_Handle)
    {
        return BEE_NOTHING_TO_DO;
    }

    if (DestroyWindow(m_Handle))
    {
        m_Handle = NULL;

        return BEE_SUCCESS;
    }
    else
    {
        B_WIN_REPORT_FAILURE();
        BEE_LOG(Debug::Error,
                L"Couldn't destroy the window %p, with index %d.",
                this,
                this->GetIndex());

        return BEE_CORRUPTION;
    }
}

void IWindow::RegisterInManager()
{
    SetIndex(Manager::Get().Register(this));
}

void IWindow::UnRegisterInManager()
{
    if (BEE_FAILED(Manager::Get().UnRegister(this)))
        throw Debug::Exception(L"A IWindow couldn't UnRegister itself", BEE_COLLECT_DATA_ON_EXCEPTION());
}
