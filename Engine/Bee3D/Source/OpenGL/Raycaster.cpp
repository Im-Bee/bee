#include "Bee3D.hpp"
#include "../Include/OpenGL/RaycasterGL.hpp"

using namespace Bee;
using namespace Bee::GL;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;

b_status RaycasterRenderer::Initialize()
{
    if (BEE_FAILED(m_Window.Initialize()))
    {
        BEE_RETURN_FAIL;
    }

    if (BEE_FAILED(m_Window.Show()))
    {
        BEE_RETURN_FAIL;
    }

    if (BEE_FAILED(LoadPipeline()))
    {
        BEE_RETURN_BAD;
    }

    BEE_RETURN_SUCCESS;
}

void RaycasterRenderer::Update()
{
    if (BEE_FAILED(ResizeScene()))
    {
        throw ::Bee::Debug::Exception(L"Could't resize!", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
}

void RaycasterRenderer::Render()
{
    const auto& width  = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height = static_cast<GLsizei>(m_WindowDim.y);

    BEE_GL(glClear(GL_COLOR_BUFFER_BIT));

    for (GLsizei i = 0; i < width * height * 4; ++i)
    {
        m_vPixels[i] = i % 255;
    }
    BEE_GL(glDrawPixels(width, 
                        height, 
                        GL_RGBA, 
                        GL_UNSIGNED_BYTE, 
                        &m_vPixels));

    if (!SwapBuffers(m_Window.GetHDC()))
    {
        B_WIN_REPORT_FAILURE();
        throw ::Bee::Debug::Exception(L"Could't swap buffers!", BEE_COLLECT_DATA_ON_EXCEPTION());
    }
}

b_status RaycasterRenderer::Destroy()
{
    if (m_Window.GetHandle())
    {
        m_Window.Destroy();
    }

    BEE_RETURN_SUCCESS;
}

b_status RaycasterRenderer::LoadPipeline()
{
    BEE_GL(glShadeModel(GL_SMOOTH));
    
    BEE_GL(glClearColor(0.f, 0.f, 0.f, 1.f));
    BEE_GL(glClearDepth(1.));

    BEE_GLEW(glewInit());

    if (BEE_FAILED(ResizeScene()))
    {
        BEE_RETURN_BAD;
    }

    BEE_RETURN_SUCCESS;
}

b_status RaycasterRenderer::ResizeScene()
{
    m_WindowDim = m_Window.GetCurrentDimensions();
    const auto& width  = static_cast<GLsizei>(m_WindowDim.x);
    const auto& height = static_cast<GLsizei>(m_WindowDim.y);

    if (width == 0 || height == 0)
    {
        BEE_RETURN_FAIL;
    }

    m_vPixels.SetCapacity(width * height * static_cast<b_usize>(4));

    BEE_GL(glViewport(0, 0, width, height));

    BEE_GL(glMatrixMode(GL_MODELVIEW));
    BEE_GL(glLoadIdentity());

    BEE_GL(gluOrtho2D(0., 
                      static_cast<double>(width), 
                      static_cast<double>(height), 
                      0.));

    BEE_RETURN_SUCCESS;
}
