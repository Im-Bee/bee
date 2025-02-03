#include "Bee3D.hpp"
#include "../Include/OpenGL/OpenGLWindow.hpp"

using namespace Bee::App::Primitives;
using namespace Bee::GL;
using namespace Bee::Utils;

b_status Bee::GL::OpenGLWindow::Initialize()
{
    if (BEE_IS_CORRUPTED(IWindow::Initialize()))
    {
        throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    if (BEE_IS_CORRUPTED(SetUpOpenGLContext()))
    {
        throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());
    }

    return BEE_SUCCESS;
}

b_status OpenGLWindow::Destroy()
{
    ReleaseOpenGLContext();

    return IWindow::Destroy();
}

b_status OpenGLWindow::SetUpOpenGLContext()
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

        return BEE_CORRUPTION;
    }

    if (!(pixelFormat = ChoosePixelFormat(m_HDC, &pfd)))
    {
        B_WIN_REPORT_FAILURE();
        
        return BEE_CORRUPTION;
    }

    if (!SetPixelFormat(m_HDC, pixelFormat, &pfd))
    {
        B_WIN_REPORT_FAILURE();

        return BEE_CORRUPTION;
    }

    if (!(m_HRC = wglCreateContext(m_HDC)))          
    {
        B_WIN_REPORT_FAILURE();

        return BEE_CORRUPTION;
    }
    
    if (!wglMakeCurrent(m_HDC, m_HRC))
    {
        B_WIN_REPORT_FAILURE();

        return BEE_CORRUPTION;
    }

    SetForegroundWindow(m_Handle);
    SetFocus(m_Handle);

    return BEE_SUCCESS;
}

b_status OpenGLWindow::ReleaseOpenGLContext()
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

    return BEE_ALREADY_DID;
}
