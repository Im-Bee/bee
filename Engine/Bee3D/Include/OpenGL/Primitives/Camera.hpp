#pragma once

namespace Bee::GL
{
    class BEE_API Camera
    {
        template<class T> using SharedPtr = ::Bee::Utils::SharedPtr<T>;
                          using Vec3f     = ::Bee::Utils::Vec3f;

        friend ::Bee::Utils::SharedBlock<Camera>;

        Camera() 
        : m_vPos({ 0, 0 }) 
        {};

    public:
        ~Camera() = default;

        static SharedPtr<Camera> Create();

    public:
        void Move(const Vec3f& pos);

    public:
        Vec3f GetPos() { return m_vPos; }

    private:
        Vec3f m_vPos = { BEE_INFINITY, BEE_INFINITY };
    };
}