#pragma once

#include "OpenGLWindow.hpp"

#include "Primitives/Camera.hpp"

namespace Bee::GL
{
    class BEE_API RendererGL
    {
        template<class T> using SharedPtr      = ::Bee::Utils::SharedPtr<T>;
                          using Status         = ::Bee::Utils::b_status;
                          using PixelVector    = ::Bee::Utils::DynamicArray<unsigned char>;
                          using Rectangle      = ::Bee::Utils::Rectangle;
                          using Vec2f          = ::Bee::Utils::Vec2<float>;
                          using Vec3Byte       = ::Bee::Utils::Vec3<unsigned char>;
                          using Vec3f          = ::Bee::Utils::Vec3f;
                          using Triangle3f     = ::Bee::Utils::Triangle3f;
                          using TriangleVector = ::Bee::Utils::Vector<::Bee::Utils::Triangle3f>;

    public:
        RendererGL() = default;
        ~RendererGL()
        {
            Destroy();
        }

    public:
        OpenGLWindow& GetWindow() { return m_Window; }

    public:
        void LoadMeshFromObj(const wchar_t* wszPath);

        void SetMainCamera(SharedPtr<Camera> pCamera)
        {
            BEE_LOG(::Bee::Debug::Info, L"Setting main camera to %p", pCamera.Get());
            m_pMainCamera = pCamera;
        }

    public:
        Status Initialize();
        Status Update();
        Status Render();
        Status Destroy();

#pragma region Debug Keybinds
    public:
        static void Aciton(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f w(.0f, .0f, .1f);

            p->m_pMainCamera->Move(w);
        }

        static void Aciton2(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f a(-.1f, .0f, .0f);

            p->m_pMainCamera->Move(a);
        }

        static void Aciton3(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f d(.1f, 0.0f, .0f);

            p->m_pMainCamera->Move(d);
        }

        static void Aciton4(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f s(.0f, .0f, -.1f);

            p->m_pMainCamera->Move(s);
        }

        static void Aciton5(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f z(.0f, .1f, .0f);

            p->m_pMainCamera->Move(z);
        }

        static void Aciton6(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f x(.0f, -.1f, .0f);

            p->m_pMainCamera->Move(x);
        }

        static void Aciton7(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static float e(-5.f);

            p->m_pMainCamera->Rotate(e);
        }

        static void Aciton8(void* pObj)
        {
            auto p = reinterpret_cast<RendererGL*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static float q(5.f);

            p->m_pMainCamera->Rotate(q);
        }
#pragma endregion

    private:
        Status LoadPipeline();
        Status ReSizeScene();

    private:
        ::Bee::GL::OpenGLWindow m_Window = {};

        GLuint m_uShaderProgram = -1;
        GLuint m_uVA = -1;
        GLuint m_uVB = -1;

        float  m_dT  = BEE_INFINITY;

        SharedPtr<Camera> m_pMainCamera = nullptr;
        TriangleVector    m_vTriangles  = {};
    };
}