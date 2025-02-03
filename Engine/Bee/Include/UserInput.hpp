#pragma once

namespace Bee::App
{
    typedef ::Bee::Utils::Pair<void(*)(void*), void*> Action;
    typedef ::Bee::Utils::Pair<Action, int> Keybind;

#pragma warning(push)
    // Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Input
    {
        using Status         = ::Bee::Utils::b_status;
        using KeybindsVector = ::Bee::Utils::Vector<Keybind>;

        static Input* m_pInstance;
        Input() = default;

    public:
        ~Input() = default;

        static Input& Get();

    public:
        Status Initialize();
        Status Destroy();

        void AttachAciton(int key, void(action)(void*), void* pObj);

    private:
        static ::LRESULT CALLBACK KeyboardProc(_In_ int    nCode,
                                               _In_ WPARAM wParam,
                                               _In_ LPARAM lParam);

    private:
        ::HHOOK        m_Hook      = NULL;
        KeybindsVector m_vKeybinds = {};

    };
#pragma warning(pop)
}