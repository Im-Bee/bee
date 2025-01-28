#pragma once

#include "OpenGLWindow.hpp"

#include "Primitives/Camera.hpp"

namespace Bee::GL
{
    struct RayHit
    {
        using Vec3f = ::Bee::Utils::Vec3f;

        Vec3f Entry;
        Vec3f Exit;
    };

    class BEE_API RaycasterRenderer
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
        RaycasterRenderer()
        : m_Window(),
          m_vPixels()
        {};

        ~RaycasterRenderer() = default;

    public:
        OpenGLWindow& GetWindow() { return m_Window; }

    public:
        void LoadMeshFromObj(const wchar_t* wszPath);

    public:
        Status Initialize();
        void   Update();
        void   Render();
        Status Destroy();

    public:
        void SetMainCamera(SharedPtr<Camera> pCamera)
        {
            BEE_LOG(::Bee::Debug::Info, L"Setting main camera to %p", pCamera.Get());
            m_pMainCamera = pCamera;
        }

#pragma region Debug Keybinds
    public:
        static void Aciton(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f w(.0f, .0f, 100.f);

            p->m_pMainCamera->Move(w);
        }

        static void Aciton2(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f a(-100.0f, .0f, .0f);

            p->m_pMainCamera->Move(a);
        }

        static void Aciton3(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f d(100.0f, 0.0f, .0f);

            p->m_pMainCamera->Move(d);
        }

        static void Aciton4(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f s(.0f, .0f, -100.0f);

            p->m_pMainCamera->Move(s);
        }

        static void Aciton5(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f z(.0f, 100.0f, .0f);

            p->m_pMainCamera->Move(z);
        }

        static void Aciton6(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static Vec3f x(.0f, -100.0f, .0f);

            p->m_pMainCamera->Move(x);
        }

        static void Aciton7(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static float e(5.f);

            p->m_pMainCamera->Rotate(e);
        }

        static void Aciton8(void* pObj)
        {
            auto p = reinterpret_cast<RaycasterRenderer*>(pObj);
            if (!p->m_pMainCamera.Get())
            {
                return;
            }
            static float q(-5.f);

            p->m_pMainCamera->Rotate(q);
        }
#pragma endregion

    private:
        Status LoadPipeline();
        Status ResizeScene();

        RayHit CastRay(const Vec3f& origin, const Vec3f& rayVector);

        Vec3f  RayIntersectsTriangle(const Vec3f& origin,
                                     const Vec3f& rayVector,
                                     const Triangle3f& triangle);

        void   PaintPixel(const Vec2f& Coords, const Vec3Byte& Color);

    private:
        OpenGLWindow      m_Window      = {};
        PixelVector       m_vPixels     = {};
        TriangleVector    m_vTriangles  = {};
        Rectangle         m_WindowDim   = Rectangle({ BEE_INFINITY, BEE_INFINITY });
        SharedPtr<Camera> m_pMainCamera = nullptr;
    };
}