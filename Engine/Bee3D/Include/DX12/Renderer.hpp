#pragma once

namespace Bee::DX12
{
    class BEE_API Renderer
    {
    public:
        Renderer() = default;
        ~Renderer()
        {
            this->Destroy();
        }

    public:
        Bee::Utils::b_success Initialize();
        void Update();
        void Render();
        Bee::Utils::b_success Destroy();

    private:


    };
}
