#include "Bee.hpp"

#include "Interfaces/IWindow.hpp"

using namespace Bee::Utils;
using namespace Bee::App;

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
