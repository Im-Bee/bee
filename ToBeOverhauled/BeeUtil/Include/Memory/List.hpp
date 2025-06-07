#pragma once

#include "Memory.hpp"

namespace Bee::Utils
{
    template<class T, 
             class Alloc = ::Bee::Utils::Memory::Allocator<T, 8, 4>>
    class UnorderedList
    {
        using uMemAddrsInt = ::Bee::Utils::Memory::b_usize;
        using Iterator     = ::Bee::Utils::Memory::Iterator<T>;

        Alloc        m_Allocator = {};
        uMemAddrsInt m_uPosition = 0;

    public:
        UnorderedList()  = default;
        ~UnorderedList()
        {
            for (uMemAddrsInt i = 0; i < m_uPosition; ++i)
                m_Allocator[i].~T();
        }

// Getters --------------------------------------------------------------------
    public:
        const uMemAddrsInt& GetCapacity() const { return m_Allocator.GetCapacity(); }

        const uMemAddrsInt& GetSize() const { return m_uPosition; }

        Iterator GetBegin() { return m_Allocator.GetBegin(); }

        Iterator GetEnd() { return m_Allocator.GetBegin() + m_uPosition; }

        T& GetFirst() { return m_Allocator[0]; }

        T& GetLast() { return m_Allocator[m_uPosition - 1]; }

// Setters --------------------------------------------------------------------
    public:
        void SetCapacity(const uMemAddrsInt& size)
        {
            m_Allocator.SetSize(size * sizeof(T));
        }

// Public Methods -------------------------------------------------------------
    public:
        T& Push(const T& item)
        {
            if (m_uPosition >= m_Allocator.GetCapacity())
            {
                m_Allocator.Resize();
            }

            return m_Allocator[m_uPosition++] = item;
        }

        T& Push(T&& item)
        {
            if (m_uPosition >= m_Allocator.GetCapacity())
            {
                m_Allocator.Resize();
            }

            return MoveOnConstruct(&m_Allocator[m_uPosition++], Utils::Memory::Move(item));
        }

        void Pop()
        {
            m_Allocator[--m_uPosition].~T();
        }

        void Pop(const uMemAddrsInt& index)
        {
            m_Allocator[index].~T();
            m_Allocator[index] = m_Allocator[--m_uPosition];
        }

// Operators ------------------------------------------------------------------
    public:
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
