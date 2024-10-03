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
        uint64_t m_Index = B_WINDOW_UNKOWN_INDEX;
        HWND m_Handle = NULL;

    public:
        IWindow()
        {
            RegisterInManager();
        }
        
        ~IWindow() = default;
         
        IWindow(IWindow&&) = default;
        IWindow(const IWindow&) = default;

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
        WNDCLASSEX GetBaseWndClassEX()
        {
            WNDCLASSEX wcex = {};

            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.lpfnWndProc = IWindow::WindowProc;
            wcex.hInstance = B_HINSTANCE();

            return wcex;
        }

        void SetHandle(HWND handle) { m_Handle = handle; }

        void SetIndex(uint64_t i) { m_Index = i; }

    public:
        virtual Bee::Utils::b_success Initialize() = 0;
        virtual Utils::b_success Show() = 0;
        virtual Utils::b_success Hide() = 0;
        virtual Bee::Utils::b_success Destroy() = 0;
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
                if (uMsg == WM_DESTROY)
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

    class BEE_API EmptyWindow : public Bee::App::IWindow
    {
        const WindowProperties m_BaseSettings;

    public:
        EmptyWindow() : m_BaseSettings(WindowProperties()) {};
        explicit EmptyWindow(WindowProperties baseSettings) : m_BaseSettings(baseSettings) {};
        ~EmptyWindow()
        {
            this->Destroy();
        }

        // EmptyWindow(EmptyWindow&&) noexcept;
        // EmptyWindow(const EmptyWindow&) noexcept;

    public:
        virtual Bee::Utils::b_success Initialize() override;

        virtual Utils::b_success Show() override;

        virtual Utils::b_success Hide() override;

        virtual Bee::Utils::b_success Destroy() override;

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            return DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
        }
    };
}