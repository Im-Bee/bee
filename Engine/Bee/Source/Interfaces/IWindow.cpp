#include "Bee.hpp"

#include "Interfaces/IWindow.hpp"

Bee::Utils::b_status Bee::App::IWindow::Destroy()
{
    HWND handle;
    B_GET_HANDLE(handle);

    if (DestroyWindow(handle))
    {
        B_RETURN_SUCCESS;
    }
    else
    {
        B_LOG(
            Bee::Problems::Error,
            L"Couldn't destroy the window %p, with index %d.",
            this,
            this->GetIndex());

        B_RETURN_BAD;
    }
}

void Bee::App::IWindow::RegisterInManager()
{
    SetIndex(Bee::App::Manager::Get().Register(this));
}

void Bee::App::IWindow::UnRegisterInManager()
{
    if (B_IS_FAIL(Bee::App::Manager::Get().UnRegister(this)))
        throw Problems::Exception(L"A IWindow couln't UnRegister itself", B_COLLECT_DATA());
}
