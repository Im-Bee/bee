#include "Bee.hpp"

#include "../Include/UserInput.hpp"

using namespace Bee::App;
using namespace Bee::Utils;

Input* Input::m_pInstance = new Input();

Input& Bee::App::Input::Get()
{
    return *m_pInstance;
}

b_status Input::Initialize()
{
    m_Hook = SetWindowsHookEx(WH_KEYBOARD,
                              &Input::KeyboardProc,
                              B_HINSTANCE(),
                              GetCurrentThreadId());

    if (m_Hook == NULL)
    {
        B_WIN_REPORT_FAILURE();
        
        return BEE_CORRUPTION;
    }
      
    return BEE_SUCCESS;
}

b_status Input::Destroy()
{
    if (m_Hook != NULL)
    {
        if (!UnhookWindowsHookEx(m_Hook))
        {
            BEE_LOG(Debug::Error, L"Input (%p) couldn't unhook itself from windows", this);

            m_Hook = NULL;

            return BEE_CORRUPTION;
        }
    }

    return BEE_SUCCESS;
}

void Input::AttachAciton(int key, void(action)(void*), void* pObj)
{
    Action ac;
    ac.a = action;
    ac.b = pObj;

    Keybind kb;
    kb.a = ac;
    kb.b = key;

    m_vKeybinds.Push(kb);
}

::LRESULT CALLBACK Input::KeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (nCode < 0)
    {
        return CallNextHookEx(NULL, nCode, wParam,   lParam);
    }

    Input& rInput = Input::Get();

    for (Utils::Memory::b_usize i = 0; i < rInput.m_vKeybinds.GetSize(); ++i)
    {
        if (rInput.m_vKeybinds[i].b == wParam)
        {
            rInput.m_vKeybinds[i].a.a(rInput.m_vKeybinds[i].a.b);   
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
