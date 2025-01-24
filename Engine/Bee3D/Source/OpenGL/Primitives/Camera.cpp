#include "Bee3D.hpp"

#include "../Include/OpenGL/Primitives/Camera.hpp"

using namespace Bee::GL;   
using namespace Bee::Utils;


SharedPtr<Camera> Camera::Create()
{
    return MakeShared<Camera>();
}

void Camera::Move(const Vec3f& pos)
{
    m_vPos += pos;
}

void Camera::Rotate(const float& rot)
{
    m_fYRotation += rot * BEE_DEG_TO_RADIAN;

    while (m_fYRotation > BEE_TWO_PI)
    {
        m_fYRotation -= BEE_TWO_PI;
    }

    while (m_fYRotation < 0)
    {
        m_fYRotation += BEE_TWO_PI;
    }
}