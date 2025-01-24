#pragma once

namespace Bee::GL
{
    class BEE_API Camera
    {
        template<class T> using SharedPtr = ::Bee::Utils::SharedPtr<T>;
                          using Vec3f     = ::Bee::Utils::Vec3f;

        friend ::Bee::Utils::SharedBlock<Camera>;

        Camera() 
        : m_vPos({ 0.f, 0.f }),
          m_fYRotation(0.f)
        {};

    public:
        ~Camera() = default;

        static SharedPtr<Camera> Create();

    public:
        void Move(const Vec3f& pos);

        void Rotate(const float& rot);

    public:
        const Vec3f& GetPos()       const { return m_vPos; }
        const float& GetYRotation() const { return m_fYRotation; }

    private:
        Vec3f m_vPos       = { BEE_INFINITY, BEE_INFINITY };
        float m_fYRotation = BEE_INFINITY;
    };
}