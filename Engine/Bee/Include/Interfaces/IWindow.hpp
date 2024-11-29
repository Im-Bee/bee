#pragma once

#define B_WINDOW_UNKOWN_INDEX      (uint64_t(-1))
#define B_WINDOW_MAIN_WINDOW_INDEX (uint64_t( 0))

namespace Bee::App
{
    struct BEE_API WindowProperties
    {
        const wchar_t* Title	  = L"Unknown";
        const wchar_t* Class	  = L"Unknown";
        Rectangle      Dimensions = {
            1200, /* Width  */
             700  /* Height */ };
    };

    class BEE_API IWindow
    {
        using status = Bee::Utils::b_status;

    public:
                 IWindow();
        explicit IWindow(WindowProperties);
        explicit IWindow(WindowProperties&&);
        
        ~IWindow();
         
    public:
        virtual status  Initialize() = 0;
                status  Show();
                status  Hide();
                status  Destroy();

    public:
        const HWND&             GetHandle()            const { return m_Handle; }
        const uint64_t&         GetIndex()             const { return m_Index; }
        const WindowProperties& GetProperties()        const { return m_BaseSettings; }
        Rectangle               GetCurrentDimensions() const;
        Rectangle               GetCurrentPos()        const;

    public:
        void SwapIndex(Bee::App::IWindow*);

        void MoveFrame(const Rectangle& = Rectangle(0,0));

        void SetDimension(const Rectangle&);

    protected:
        WNDCLASSEX GetBaseWndClassEX() const
        {
            WNDCLASSEX wcex = {};

            wcex.cbSize      = sizeof(WNDCLASSEX);
            wcex.lpfnWndProc = IWindow::WindowProc;
            wcex.hInstance   = B_HINSTANCE();

            return wcex;
        }

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    protected:
        void SetHandle(HWND handle) { m_Handle = handle; }
        void SetIndex(uint64_t i)   { m_Index = i; }

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            IWindow* pThis = nullptr;

            if (uMsg == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                pThis = (IWindow*)pCreate->lpCreateParams;

                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

                pThis->SetHandle(hwnd);
            }
            else
            {
                pThis = (IWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            }

            if (pThis)
            {
                if (uMsg == WM_NCDESTROY)
                    pThis->UnRegisterInManager();

                return pThis->HandleMessage(uMsg, wParam, lParam);
            }
            else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }

        void RegisterInManager();
        void UnRegisterInManager();

    protected:
        uint64_t m_Index = B_WINDOW_UNKOWN_INDEX;
        HWND m_Handle = NULL;
        const WindowProperties m_BaseSettings;
    };
}