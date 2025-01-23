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
    m_vPos = m_vPos + pos;
}
