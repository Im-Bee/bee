#include "Bee3D.hpp"

#include "../Include/OpenGL/RendererGL.hpp"

using namespace Bee;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;
using namespace Bee::Debug;


float points[] = {
   0.9f,  0.9f,  0.0f, 0.0f,
   0.9f, -0.9f,  0.0f, 0.0f,
  -0.9f, -0.9f,  0.0f, 0.0f,
  -0.9f,  0.9f,  0.0f, 0.0f,
};

typedef Vec4<float> Vertices;

struct MeshData
{
    Vector<Vertices> VertexData;  // v
};

//-----------------------------------------------------------------------------
//          LoadObj by ATL
//-----------------------------------------------------------------------------

void LoadObj(const wchar_t* wszFilePath)
{
    MeshData md   = {};
    auto filebuff = App::Manager::Get().ReadFile(wszFilePath); 

    for (b_uintmem i = 0; i < filebuff.Size; ++i)
    {
        auto& c = filebuff.Buffer[i];

        if (ToLower(c) == 'v')
        {
            Vertices v;
            ScanLine(&c, filebuff.Size - i - 1, "v %f %f %f", v.x, v.y, v.z);
            v.w = 1.0f;

            md.VertexData.Push(v);
        }
    }









    auto iter = md.VertexData.GetBegin();
    while (iter != md.VertexData.GetEnd())
    {
        BEE_LOG(Info, L"v %f %f %f %f", iter->x, iter->y, iter->z, iter->w);
        ++iter;
    }
}

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

    static float a = 1.5;
    loc = glGetUniformLocation(m_uShaderProgram, "iTime");
    // a += 0.02f;
    glUniform1f(loc, a);
    
    glLoadIdentity();

    glBindBuffer(GL_ARRAY_BUFFER, m_uVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::RendererGL::Render()
{
    if (!m_Window.GetHandle())
    {
        BEE_RETURN_OKAY;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(m_uShaderProgram);
    glBindVertexArray(m_uVA);
    glDrawArrays(GL_QUADS, 0, sizeof(points) / 4);

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
        BEE_RETURN_OKAY;
    }

    glColor3f(0.0, 1.0, 0.0);



    glGenBuffers(1, &m_uVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_uVA);
    glBindVertexArray(m_uVA);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVB);
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
        BEE_LOG(Debug::Error, L"%S", log);
    }

    wchar_t wszTestMeshPath[BEE_MAX_PATH] = { 0 };
    wcscpy_s(wszTestMeshPath, App::Properties::Get().GetResourcesPath());
    wcscat_s(wszTestMeshPath, L"Meshes\\Empty.obj");
    LoadObj(wszTestMeshPath);

    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::RendererGL::ReSizeScene()
{
    const auto dim     = m_Window.GetCurrentDimensions();
    const auto& width  = dim.x;
    const auto& height = dim.y;

    if (!width || !height)
    {
        BEE_RETURN_FAIL;
    }

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);                   
    glLoadIdentity();                              

    gluPerspective(59.5f, width / height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                        
    glLoadIdentity();

    gluOrtho2D(-780, 780, -420, 420);

    BEE_RETURN_SUCCESS;
}


