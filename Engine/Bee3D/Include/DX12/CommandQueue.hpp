#pragma once

namespace Bee::DX12
{
    class Renderer;

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API CommandQueue : public IComponent
    {
        template<class T> using ComPtr = Bee::Utils::ComPtr<T>;
                          using Status = Bee::Utils::b_status;

    public:
        CommandQueue() = default;
        ~CommandQueue() = default;

        CommandQueue(CommandQueue&&) = default;
    };
#pragma warning(pop)
}