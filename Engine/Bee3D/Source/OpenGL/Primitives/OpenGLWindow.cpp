#include "Bee3D.hpp"

using namespace Bee::Utils;
using namespace Bee::App::Primitives;

b_status Bee::GL::Primitives::OpenGLWindow::Initialize()
{
    if (BEE_CORRUPTED(IWindow::Initialize()))
        throw Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());

    if (BEE_CORRUPTED(SetUpOpenGLContext()))
        throw Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());

    BEE_RETURN_SUCCESS;
}

Bee::Utils::b_status Bee::GL::Primitives::OpenGLWindow::Destroy()
{
    ReleaseOpenGLContext();

    return IWindow::Destroy();
}

b_status Bee::GL::Primitives::OpenGLWindow::SetUpOpenGLContext()
{
    int pixelFormat;

    static PIXELFORMATDESCRIPTOR pfd =             
    {
        sizeof(PIXELFORMATDESCRIPTOR),           
        1,                                       
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,                        
        PFD_TYPE_RGBA,                           
        32,                                      
        0, 0, 0, 0, 0, 0,                        
        0,                                       
        0,                                       
        0,                                       
        0, 0, 0, 0,                              
        16,                                      
        0,                                       
        0,                                       
        PFD_MAIN_PLANE,                          
        0,                                       
        0, 0, 0                                  
    };                                           
                                                 
    if (!(m_HDC = GetDC(m_Handle)))              
    {
        B_WIN_REPORT_FAILURE();
        BEE_RETURN_BAD;
    }

    if (!(pixelFormat = ChoosePixelFormat(m_HDC, &pfd)))
    {
        B_WIN_REPORT_FAILURE();
        BEE_RETURN_BAD;
    }

    if (!SetPixelFormat(m_HDC, pixelFormat, &pfd))
    {
        B_WIN_REPORT_FAILURE();
        BEE_RETURN_BAD;
    }

    if (!(m_HRC = wglCreateContext(m_HDC)))          
    {
        B_WIN_REPORT_FAILURE();
        BEE_RETURN_BAD;
    }
    
    if (!wglMakeCurrent(m_HDC, m_HRC))
    {
        B_WIN_REPORT_FAILURE();
        BEE_RETURN_BAD;
    }

    SetForegroundWindow(m_Handle);
    SetFocus(m_Handle);

    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::Primitives::OpenGLWindow::ReleaseOpenGLContext()
{
    if (m_HRC)
    {
        if (!wglMakeCurrent(NULL, NULL))
        {
            B_WIN_REPORT_FAILURE();
        }

        if (!wglDeleteContext(m_HRC))
        {
            B_WIN_REPORT_FAILURE();
        }

        m_HRC = NULL;
    }

    if (m_HDC)
    {
        if (!ReleaseDC(m_Handle, m_HDC))
        {
            B_WIN_REPORT_FAILURE();
        }
        m_HDC = NULL;
    }

    BEE_RETURN_OKAY;
}
