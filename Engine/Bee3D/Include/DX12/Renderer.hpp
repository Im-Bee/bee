#pragma once

namespace Bee::DX12
{
    class BEE_API Renderer
    {
        using status = Bee::Utils::b_success;

    public:
        Renderer() = default;
        ~Renderer()
        {
            this->Destroy();
        }

    public:
        status Initialize();
        void Update();
        void Render();
        status Destroy();

    private:


    };
}
