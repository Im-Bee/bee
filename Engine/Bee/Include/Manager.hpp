#pragma once

#define B_CREATE_WINDOW(x)  (dynamic_cast<Bee::App::IWindow*>(new x()))

namespace Bee::App
{
    constexpr bool CloseOnNoWindow = true;

    class BEE_API Manager
    {
        friend Bee::App::IWindow;

        Bee::Utils::Memory::UnorderedList<Bee::App::IWindow*> m_Windows = {};

        Manager() = default;

    public:
        ~Manager() = default;

        Manager(Manager&&) = delete;
        Manager(const Manager&) = default;


        static Manager& Get()
        {
            static Manager instance = {};
            return instance;
        }

    private:
        Bee::App::IWindow* Register(Bee::App::IWindow* wnd);

        bool UnRegister(Bee::App::IWindow* wnd);

    };
}
