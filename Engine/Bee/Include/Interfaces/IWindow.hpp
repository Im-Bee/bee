#pragma once

namespace Bee::App
{
    struct BEE_API WindowProperties
    {
        using DimensionsVec = Bee::Utils::Vec2<uint32_t>;

        const wchar_t* Title	  = L"Unknown";
        const wchar_t* Class	  = L"Unknown";
        DimensionsVec  Dimensions = { 
            1200, /* Width  */
             700  /* Height */ };
    };

    class BEE_API IWindow
    {
        HWND m_Handle = NULL;

    public:
        IWindow() = default;
        ~IWindow() = default;
         
        IWindow(IWindow&&) = default;
        IWindow(const IWindow&) = default;

    public:
        const HWND& GetHandle() { return m_Handle; }

    protected:
        WNDCLASSEX GetBaseWndClassEX()
        {
            WNDCLASSEX wcex = {};

            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.lpfnWndProc = IWindow::WindowProc;
            wcex.hInstance = B_HINSTANCE();

            return wcex;
        }

        void SetHandle(HWND handle) { m_Handle = handle; }

    public:
        virtual void Initialize() = 0;
        virtual bool Show() = 0;
        virtual bool Hide() = 0;
        virtual void Destroy() = 0;
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            IWindow* pThis = nullptr;

            if (uMsg == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                pThis = (IWindow*)pCreate->lpCreateParams;

                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

                pThis->SetHandleAndRegister(hwnd, pThis);
            }
            else
            {
                pThis = (IWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            }

            if (pThis)
            {
                if (uMsg == WM_DESTROY)
                    pThis->UnRegisterInManager(pThis);

                return pThis->HandleMessage(uMsg, wParam, lParam);
            }
            else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }

        void SetHandleAndRegister(HWND handle, Bee::App::IWindow* self);
        void UnRegisterInManager(Bee::App::IWindow* self);

    };

    class BEE_API EmptyWindow : public Bee::App::IWindow
    {
        const WindowProperties m_BaseSettings;

    public:
        EmptyWindow() : m_BaseSettings(WindowProperties()) {};
        EmptyWindow(WindowProperties&& baseSettings) : m_BaseSettings(baseSettings) {};
        ~EmptyWindow()
        {
            this->Destroy();
        }

        // EmptyWindow(EmptyWindow&&) noexcept;
        // EmptyWindow(const EmptyWindow&) noexcept;

    public:
        virtual void Initialize() override;

        virtual bool Show() override;

        virtual bool Hide() override;

        virtual void Destroy() override;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }
    };
}