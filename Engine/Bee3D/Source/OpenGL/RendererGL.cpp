#include "Bee3D.hpp"

using namespace Bee::Utils;

static const char* _VertexDebugShader = 
    "#version 420 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


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
    Vec2<float> dimsVec = {
        static_cast<float>(dim.x),
        static_cast<float>(dim.y)
    };
    glUniform2f(loc, dimsVec.x, dimsVec.y);

    static float a = 0;
    loc = glGetUniformLocation(m_uShaderProgram, "iTime");
    a += 0.08f;
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
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
       0.0f,  0.9f,  0.0f,
       0.9f, -0.9f,  0.0f,
      -0.9f, -0.9f,  0.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_uVA);
    glBindVertexArray(m_uVA);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &_VertexDebugShader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    wchar_t* wszFragmentShaderPath = App::Properties::Get().GetResourcesPath();
    wcscat_s(wszFragmentShaderPath, BEE_MAX_PATH, L"\\Shaders\\FragmentShader.glsl");
    auto fragmentShaderData = Bee::App::Manager::Get().ReadFile(wszFragmentShaderPath);

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
