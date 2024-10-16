#pragma once

#define B_CREATE_WIN_DIM(x, y) (Bee::App::WindowProperties::DimensionsVec({ x, y }))
#define B_CREATE_WIN_PROPS(title, clss, dim) (Bee::App::WindowProperties({ title, clss, dim }))
#define B_CREATE_WIN(winClass, title, clss, dim) (new winClass( B_CREATE_WIN_PROPS(title, clss, dim) ) )

#define B_WINDOW_UNKOWN_INDEX ((uint64_t)(-1))
#define B_WINDOW_MAIN_WINDOW_INDEX ((uint64_t)(0))

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
        using status = Bee::Utils::b_status;

        uint64_t m_Index = B_WINDOW_UNKOWN_INDEX;
        HWND m_Handle    = NULL;

    public:
        IWindow()
        {
            RegisterInManager();
        }
        
        ~IWindow()
        {
            if (m_Handle != NULL)
                Destroy();
        }
         
    public:
        const HWND& GetHandle() const { return m_Handle; }

        const uint64_t& GetIndex() const { return m_Index; }

        void SwapIndex(Bee::App::IWindow* other)
        {
            auto tmp = other->GetIndex();
            other->m_Index = this->m_Index;
            this->m_Index = tmp;
        }

    protected:
        WNDCLASSEX GetBaseWndClassEX() const
        {
            WNDCLASSEX wcex = {};

            wcex.cbSize      = sizeof(WNDCLASSEX);
            wcex.lpfnWndProc = IWindow::WindowProc;
            wcex.hInstance   = B_HINSTANCE();

            return wcex;
        }

        void SetHandle(HWND handle) { m_Handle = handle; }

        void SetIndex(uint64_t i) { m_Index = i; }

    public:
        virtual status Initialize() = 0;
        status Show();
        status Hide();
        status Destroy();
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

    };
}