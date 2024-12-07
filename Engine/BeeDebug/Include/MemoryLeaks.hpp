#pragma once

namespace Bee::Problems
{
    class IMemoryLeaksTracker
    {
        inline static int m_uCounter = 0;

    public:
        static void Increment() { ++m_uCounter; }
        static void Decrement() { ++m_uCounter; }

    protected:
        const int& GetMemLeaksAmount() { return m_uCounter; }
    };
}