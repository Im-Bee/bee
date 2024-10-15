#pragma once

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

#include "Allocator.hpp"

namespace Bee::Utils::Memory
{
    template<
        class T, 
        class Alloc = Bee::Utils::Memory::Allocator<T, 8, 4>>
    class Vector
    {
        Alloc m_Allocation  = {};
        uintmem m_uPosition = 0;

    public:
        Vector() = default;
        ~Vector() = default;

    public:
        const uintmem& GetCapacity() const { return m_Allocation.GetCapacity(); }
        const uintmem& GetSize() const { return m_uPosition; }

        void SetCapacity(const uintmem& size)
        {
            m_Allocation.SetSize(size * sizeof(T));
        }

    public:
        T& Push(const T& item)
        {
            if (m_uPosition >= m_Allocation.GetCapacity())
                m_Allocation.Resize();

            return m_Allocation[m_uPosition++] = item;
        }

        T& Push(T&& item)
        {
            if (m_uPosition >= m_Allocation.GetCapacity())
                m_Allocation.Resize();

            return m_Allocation[m_uPosition++] = item;
        }

        void Pop()
        {
            Memory::DestroyAt(&m_Allocation[--m_uPosition]);
        }

    public:
        T& operator[](const uintmem& index) const
        {
            if (index >= m_uPosition)
                throw Problems::OutsideOfBuffer(B_COLLECT_DATA());

            return m_Allocation[index];
        }
    };
}
