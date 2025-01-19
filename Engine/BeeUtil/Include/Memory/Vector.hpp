#pragma once

#include "Memory.hpp"

namespace Bee::Utils
{
    template<class T, 
             class Alloc = Bee::Utils::Memory::Allocator<T, 8, 2>>
    class Vector
    {
        using uMemAddrsInt = ::Bee::Utils::Memory::b_uintmem;
        using Iterator     = ::Bee::Utils::Memory::Iterator<T>;

        Alloc        m_Allocator = {};
        uMemAddrsInt m_uPosition = 0;

    public:
        Vector() = default;

        Vector(Vector&& other) = default;

        ~Vector()
        {
            for (uMemAddrsInt i = 0; i < m_uPosition; ++i)
            {
                m_Allocator[i].~T();
            }
        }

    public:
// Getters --------------------------------------------------------------------
        /**
        * Returns maximum amount of bytes that can be currently stored.
        **/
        const uMemAddrsInt& GetCapacity() const { return m_Allocator.GetCapacity(); }

        /**
        * Returns amount of elements that is currently stored.
        **/
        const uMemAddrsInt& GetSize() const { return m_uPosition; }

        Iterator GetBegin() const { return m_Allocator.GetBegin(); }

        Iterator GetEnd() { return m_Allocator.GetBegin() + m_uPosition; }

    public:
// Setters --------------------------------------------------------------------
        void SetCapacity(const uMemAddrsInt& size)
        {
            m_Allocator.SetSize(size);
        }

    public:
// Public Methods -------------------------------------------------------------
        T& Push(const T& item)
        {
            if (m_uPosition >= m_Allocator.GetSize())
            {
                m_Allocator.Resize();
            }

            return m_Allocator[m_uPosition++] = item;
        }

        T& Push(T&& item)
        {
            if (m_uPosition >= m_Allocator.GetSize())
            {
                m_Allocator.Resize();
            }

            return MoveOnConstruct(&m_Allocator[m_uPosition++], Utils::Memory::Move(item));
        }

        void Pop()
        {
            m_Allocator[--m_uPosition].~T();
        }

        T& GetFirst() { return m_Allocator[0]; }

        T& GetLast() { return m_Allocator[m_uPosition - 1]; }

    public:
// Operators ------------------------------------------------------------------
        T& operator[](const uMemAddrsInt& index) const
        {
            if (index >= m_uPosition)
            {
                throw Debug::OutsideOfBuffer(BEE_COLLECT_DATA_ON_EXCEPTION());
            }

            return m_Allocator[index];
        }
    };
}
