#pragma once

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

#include "Memory.hpp"

namespace Bee::Utils::Memory
{
    template<
        class T, 
        class Alloc = Bee::Utils::Memory::Allocator<T, 8, 2>>
    class Vector
    {
        Alloc   m_Allocator = {};
        b_uintmem m_uPosition = 0;

    public:
        Vector() = default;

        Vector(Vector&& other) = default;

        ~Vector()
        {
            for (b_uintmem i = 0; i < m_uPosition; ++i)
                m_Allocator[i].~T();
        }

    public:
// Getters --------------------------------------------------------------------
        /**
        * Returns maximum amount of bytes that can be currently stored.
        **/
        const b_uintmem& GetCapacity() const { return m_Allocator.GetCapacity(); }

        /**
        * Returns amount of elements that is currently stored.
        **/
        const b_uintmem& GetSize() const { return m_uPosition; }

        Iterator<T> GetBegin() const { return m_Allocator.GetBegin(); }

    public:
// Setters --------------------------------------------------------------------
        void SetCapacity(const b_uintmem& size)
        {
            m_Allocator.SetSize(size);
        }

    public:
// Public Methods -------------------------------------------------------------
        T& Push(const T& item)
        {
            if (m_uPosition >= m_Allocator.GetSize())
                m_Allocator.Resize();

            return m_Allocator[m_uPosition++] = item;
        }

        T& Push(T&& item)
        {
            if (m_uPosition >= m_Allocator.GetSize())
                m_Allocator.Resize();

            return m_Allocator[m_uPosition++] = Utils::Memory::Move(item);
        }

        void Pop()
        {
            m_Allocator[--m_uPosition].~T();
        }

    public:
// Operators ------------------------------------------------------------------
        T& operator[](const b_uintmem& index) const
        {
            if (index >= m_uPosition)
                throw Problems::OutsideOfBuffer(BEE_COLLECT_DATA());

            return m_Allocator[index];
        }
    };
}
