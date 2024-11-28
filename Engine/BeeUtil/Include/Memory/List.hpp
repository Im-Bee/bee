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
    class UnorderedList
    {
        Alloc m_Allocator  = {};
        uintmem m_uPosition = 0;

    public:
        UnorderedList()  = default;
        ~UnorderedList()
        {
            for (uintmem i = 0; i < m_uPosition; ++i)
                m_Allocator[i].~T();
        }

    public:
// Getters --------------------------------------------------------------------
        const uintmem& GetCapacity() const { return m_Allocator.GetCapacity(); }

        const uintmem& GetSize() const { return m_uPosition; }

    public:
// Setters --------------------------------------------------------------------
        void SetCapacity(const uintmem& size)
        {
            m_Allocator.SetSize(size * sizeof(T));
        }

    public:
// Public Methods -------------------------------------------------------------
        T& Push(const T& item)
        {
            if (m_uPosition >= m_Allocator.GetCapacity())
                m_Allocator.Resize();

            return m_Allocator[m_uPosition++] = item;
        }

        T& Push(T&& item)
        {
            if (m_uPosition >= m_Allocator.GetCapacity())
                m_Allocator.Resize();

            return m_Allocator[m_uPosition++] = Utils::Move(item);
        }

        void Pop()
        {
            m_Allocator[--m_uPosition].~T();
        }

        void Pop(const uintmem& index)
        {
            m_Allocator[index].~T();
            m_Allocator[index] = m_Allocator[--m_uPosition];
        }

    public:
// Operators ------------------------------------------------------------------
        T& operator[](const uintmem& index) const
        {
            if (index >= m_uPosition)
                throw Problems::OutsideOfBuffer(BEE_COLLECT_DATA());

            return m_Allocator[index];
        }
    };
}
