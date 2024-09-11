#pragma once

namespace Bee::App
{
    class IApplication
    {
    public:
        IApplication() = default;
        ~IApplication() = default;

        IApplication(IApplication&&) = delete;
        IApplication(const IApplication&) = delete;

    public:
        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;
    };

    constexpr IApplication* EntryApplication();
}