#include "Bee3D.hpp"

#include "../Include/OpenGL/RendererGL.hpp"

using namespace Bee;
using namespace Bee::Debug;
using namespace Bee::GL;
using namespace Bee::Utils;
using namespace Bee::Utils::Memory;

void RendererGL::LoadMeshFromObj(const wchar_t* wszFilePath)
{
    enum Modes
    {
        None,
        V,
        F,
    };

    auto fileBuffer = ::Bee::App::Manager::Get().ReadFile(wszFilePath);
    Vector<Vec3f> vectors;

    Modes currentMode = None;
    b_usize lineLenght = 0;
    for (b_usize i = 0; i < fileBuffer.Size; ++i, ++lineLenght)
    {
        const char& c = fileBuffer.Buffer[i];

        if (lineLenght == 0)
        {
            if (ToLower(c) == 'v' && ToLower(*(&c + 1)) == ' ')
            {
                currentMode = V;
                continue;
            }

            if (ToLower(c) == 'f' && ToLower(*(&c + 1)) == ' ')
            {
                currentMode = F;
                continue;
            }

            currentMode = None;
            continue;
        }

        if (c != '\n' && c != '\0')
        {
            continue;
        }

        if (currentMode == V)
        {
            vectors.Push(Move(Vec3f()));
            float* xyz[] = { &vectors.GetBack().x, &vectors.GetBack().y, &vectors.GetBack().z };

            b_usize j = 1;
            for (int8_t k = 0; k < 3; ++k)
            {
                if (fileBuffer.Buffer[i - lineLenght + j] == '\0')
                {
                    break;
                }

                while (fileBuffer.Buffer[i - lineLenght + j] == ' ')
                {
                    ++j;
                }

                ScanLine(&fileBuffer.Buffer[i - lineLenght + j],
                    lineLenght - j,
                    "%f",
                    *xyz[k]);

                while (fileBuffer.Buffer[i - lineLenght + j] != ' ' &&
                    fileBuffer.Buffer[i - lineLenght + j] != '\n' &&
                    fileBuffer.Buffer[i - lineLenght + j] != '\0')
                {
                    ++j;
                }
            }
        }

        if (currentMode == F)
        {
            m_vTriangles.Push(Move(Triangle3f()));
            int32_t p0, p1, p2;
            int32_t* xyz[] = { &p0, &p1, &p2 };

            b_usize j = 1;
            for (int8_t k = 0; k < 3; ++k)
            {
                if (fileBuffer.Buffer[i - lineLenght + j] == '\0')
                {
                    break;
                }

                while (fileBuffer.Buffer[i - lineLenght + j] == ' ')
                {
                    ++j;
                }

                ScanLine(&fileBuffer.Buffer[i - lineLenght + j],
                    lineLenght - j,
                    "%d",
                    *xyz[k]);

                while (fileBuffer.Buffer[i - lineLenght + j] != ' '  &&
                       fileBuffer.Buffer[i - lineLenght + j] != '\n' &&
                       fileBuffer.Buffer[i - lineLenght + j] != '\0')
                {
                    ++j;
                }
            }

            auto& newTriangle = m_vTriangles.GetBack();

            newTriangle.p0 = vectors[p0 - 1];
            newTriangle.p1 = vectors[p1 - 1];
            newTriangle.p2 = vectors[p2 - 1];

            BEE_LOG(Debug::Info, L"%d %d %d (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)", p0, p1, p2, newTriangle.p0.x, newTriangle.p0.y, newTriangle.p0.z, newTriangle.p1.x, newTriangle.p1.y, newTriangle.p1.z, newTriangle.p2.x, newTriangle.p2.y, newTriangle.p2.z);
        }

        lineLenght = -1;
    }
}

::Bee::Utils::Mat4x4f CreatePerspectiveMatrix(float fFovY, float fAspectRatio, float fNear, float fFar)
{
    float fTanHalfFovY = tan(fFovY * .5f * BEE_DEG_TO_RADIAN);
    float fTop = fNear * fTanHalfFovY;
    float fRight = fTop * fAspectRatio;

    return Mat4x4f(fNear / fRight,          0.f,                                  0.f,  0.f,
                              .0f, fNear / fTop,                                  0.f,  0.f,
                              .0f,          0.f,     -(fFar + fNear) / (fFar - fNear), -1.f,
                              .0f,          0.f, -(2 * fFar * fNear) / (fFar - fNear),  0.f);
}

::Bee::Utils::Mat4x4f LookAt(Vec3f eye, Vec3f target, Vec3f up)
{
    Vec3f zAxis = (eye - target).Normalize();
    Vec3f xAxis = (up.CrossProduct(zAxis)).Normalize();
    Vec3f yAxis = zAxis.CrossProduct(xAxis);

    Mat4x4f orientation(xAxis.x, yAxis.x, zAxis.x, 0.f,
                        xAxis.y, yAxis.y, zAxis.y, 0.f,
                        xAxis.z, yAxis.z, zAxis.z, 0.f,
                            0.f,     0.f,     0.f, 1.f);

    Mat4x4f translation(   1.f,    0.f,    0.f, 0.f,
                           0.f,    1.f,    0.f, 0.f,
                           0.f,    0.f,    1.f, 0.f,
                        -eye.x, -eye.y, -eye.z, 1.f);

    MultiplyMat4x4(orientation, translation, translation);

    return translation;
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

    m_dT = 1.0f;

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

    loc = glGetUniformLocation(m_uShaderProgram, "iTime");
    m_dT += 0.02f;
    glUniform1f(loc, m_dT);
    
    auto perspective(CreatePerspectiveMatrix(60.f, dim.x / dim.y, 0.1f, 1000.f));
    Triangle3f* updated = new Triangle3f[m_vTriangles.GetSize() + 1];

    static float fd = 0.f;
    auto rotationMat(CreateYRotationMat3x3(fd * BEE_DEG_TO_RADIAN));
    Mat3x3f scale(2.f, 0.f, 0.f,
                  0.f, 2.f, 0.f,
                  0.f, 0.f, 2.f);

    Mat3x3f tranform;
    MultiplyMat3x3(rotationMat, scale, tranform);

    auto rot(CreateYRotationMat3x3(m_pMainCamera->GetYRotation()));
    auto target(Vec3f(0.f, 0.f, 1.f));
    MultiplyMat3x3Vec3(rot, target, target);

    auto lookAt(LookAt(m_pMainCamera->GetPos(), target, Vec3f(0.f, 1.f, 0.f)));
    TransposeMat4x4(lookAt);

    for (b_usize i = 0; i < m_vTriangles.GetSize(); ++i)
    {
        Triangle3f t;
        t.p0 = m_vTriangles[i].p0;
        t.p1 = m_vTriangles[i].p1;
        t.p2 = m_vTriangles[i].p2;
        
        MultiplyMat3x3Vec3(tranform, t.p0, t.p0);
        MultiplyMat3x3Vec3(tranform, t.p1, t.p1);
        MultiplyMat3x3Vec3(tranform, t.p2, t.p2);

        MultiplyMat4x4Vec3(lookAt, t.p0, t.p0);
        MultiplyMat4x4Vec3(lookAt, t.p1, t.p1);
        MultiplyMat4x4Vec3(lookAt, t.p2, t.p2);

        MultiplyMat4x4Vec3(perspective, t.p0, t.p0);
        MultiplyMat4x4Vec3(perspective, t.p1, t.p1);
        MultiplyMat4x4Vec3(perspective, t.p2, t.p2);

        updated[i] = t;
    }
    fd += 0.01f;
    
    if (fd > 360.f)
    {
        fd = 0.f;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_uVB);
    glBufferData(GL_ARRAY_BUFFER, m_vTriangles.GetSize() * sizeof(Triangle3f), updated, GL_STREAM_DRAW);
    delete[] updated;

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
    glDrawArrays(GL_TRIANGLES, 0, m_vTriangles.GetSize() * 3);

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
    {
        m_Window.Destroy();
    }

    BEE_RETURN_SUCCESS;
}

// ----------------------------------------------------------------------------
//                              Private Methods
// ----------------------------------------------------------------------------

b_status Bee::GL::RendererGL::LoadPipeline()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(100.0f);
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
    glBufferData(GL_ARRAY_BUFFER, m_vTriangles.GetSize() * 3 * sizeof(float), &m_vTriangles[0], GL_STREAM_DRAW);

    glGenVertexArrays(1, &m_uVA);
    glBindVertexArray(m_uVA);
    
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_uVB);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), NULL);

    wchar_t wszVertexShaderPath[BEE_MAX_PATH] = { 0 };
    wcscpy_s(wszVertexShaderPath, App::Properties::Get().GetResourcesPath());
    wcscat_s(wszVertexShaderPath, BEE_MAX_PATH, L"\\Shaders\\VertexShader.glsl");
    auto fragmentVertexData(Bee::App::Manager::Get().ReadFile(wszVertexShaderPath));

    wchar_t wszFragmentShaderPath[BEE_MAX_PATH] = { 0 };
    wcscpy_s(wszFragmentShaderPath, App::Properties::Get().GetResourcesPath());
    wcscat_s(wszFragmentShaderPath, BEE_MAX_PATH, L"\\Shaders\\FragmentShader.glsl");
    auto fragmentShaderData(Bee::App::Manager::Get().ReadFile(wszFragmentShaderPath));

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
    if (!compileStatus) 
    {
        char log[512];
        glGetProgramInfoLog(m_uShaderProgram, 512, NULL, log);
        BEE_LOG(Debug::Error, L"%S", log);
    }
    
    BEE_RETURN_SUCCESS;
}

b_status Bee::GL::RendererGL::ReSizeScene()
{
    const auto  dim    = m_Window.GetCurrentDimensions();
    const auto& width  = static_cast<int>(dim.x);
    const auto& height = static_cast<int>(dim.y);

    if (!width || !height)
    {
        BEE_RETURN_FAIL;
    }
    
    glViewport(0, 0, width, height);

    BEE_RETURN_SUCCESS;
}


