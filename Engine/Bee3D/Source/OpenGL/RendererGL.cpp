#include "Bee3D.hpp"

using namespace Bee::Utils;

// ----------------------------------------------------------------------------
//                              Public Methods
// ----------------------------------------------------------------------------

b_status Bee::GL::RendererGL::Initialize()
{
    if (BEE_FAILED(m_Window.Initialize()))
    {
        BEE_RETURN_BAD;
    }

    if (BEE_FAILED(m_Window.Show()))
    {
        BEE_RETURN_BAD;
    }

    if (BEE_FAILED(LoadPipeline()))
    {
        BEE_RETURN_BAD;
    }

    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::RendererGL::Update()
{
    Sleep(16);

    if (!m_Window.GetHandle())
    {
        BEE_RETURN_OKAY;
    }

    if (BEE_FAILED(ReSizeScene()))
    {
        BEE_RETURN_BAD;
    }

    GLint loc = glGetUniformLocation(m_uShaderProgram, "iResolution");
    const auto& dim = m_Window.GetCurrentDimensions();
    Memory::Vec2<float> dimsVec = {
        static_cast<float>(dim.x),
        static_cast<float>(dim.y)
    };
    glUniform2f(loc, dimsVec.x, dimsVec.y);

    static float a = 0;
    loc = glGetUniformLocation(m_uShaderProgram, "iTime");
    a += 0.02f;
    glUniform1f(loc, a);

    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::RendererGL::Render()
{
    if (!m_Window.GetHandle())
    {
        BEE_RETURN_OKAY;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glUseProgram(m_uShaderProgram);
    glBindVertexArray(m_uVA);
    glDrawArrays(GL_QUADS, 0, 4);

    if (!SwapBuffers(m_Window.GetHDC()))
    {
        B_WIN_REPORT_FAILURE();
        BEE_RETURN_FAIL;
    }

    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::RendererGL::Destroy()
{
    if (m_Window.GetHandle())
        m_Window.Destroy();

    BEE_RETURN_SUCCESS;
}

// ----------------------------------------------------------------------------
//                              Private Methods
// ----------------------------------------------------------------------------

b_status Bee::GL::RendererGL::LoadPipeline()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glewInit();

    if (BEE_FAILED(ReSizeScene()))
    {
        BEE_RETURN_BAD;
    }

    glColor3f(0.0, 1.0, 0.0);

    float points[] = {
       0.9f,  0.9f,  0.0f, 0.0f,
       0.9f, -0.9f,  0.0f, 0.0f,
      -0.9f, -0.9f,  0.0f, 0.0f,
      -0.9f,  0.9f,  0.0f, 0.0f,
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) * sizeof(float), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_uVA);
    glBindVertexArray(m_uVA);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    wchar_t wszVertexShaderPath[BEE_MAX_PATH] = { 0 };
    wcscpy_s(wszVertexShaderPath, App::Properties::Get().GetResourcesPath());
    wcscat_s(wszVertexShaderPath, BEE_MAX_PATH, L"\\Shaders\\VertexShader.glsl");
    auto fragmentVertexData = Bee::App::Manager::Get().ReadFile(wszVertexShaderPath);

    wchar_t wszFragmentShaderPath[BEE_MAX_PATH] = { 0 };
    wcscpy_s(wszFragmentShaderPath, App::Properties::Get().GetResourcesPath());
    wcscat_s(wszFragmentShaderPath, BEE_MAX_PATH, L"\\Shaders\\FragmentShader.glsl");
    auto fragmentShaderData = Bee::App::Manager::Get().ReadFile(wszFragmentShaderPath);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &fragmentVertexData.Buffer, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fs, 1, &fragmentShaderData.Buffer, NULL);
    glCompileShader(fs);

    m_uShaderProgram = glCreateProgram();
    glAttachShader(m_uShaderProgram, fs);

    glAttachShader(m_uShaderProgram, vs);
    glLinkProgram(m_uShaderProgram);
    GLint compileStatus;
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        char log[512];
        glGetProgramInfoLog(m_uShaderProgram, 512, NULL, log);
        BEE_LOG(Problems::Error, L"%S", log);
    }

    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::RendererGL::ReSizeScene()
{
    const auto& dim    = m_Window.GetCurrentDimensions();
    const auto& width  = dim.x;
    const auto& height = dim.y;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);                   
    glLoadIdentity();                              

    gluPerspective(59.5f, width / height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                        
    glLoadIdentity();

    gluOrtho2D(-780, 780, -420, 420);

    BEE_RETURN_SUCCESS;
}
