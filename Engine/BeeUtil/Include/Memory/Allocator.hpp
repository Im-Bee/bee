#pragma once

#include <cstdint>

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

namespace Bee::Utils::Memory
{
    typedef uint64_t uintmem;

    template<class T>
    constexpr uintmem JumpOfSize() { return sizeof(T); }

    template<class T>
    class Iterator
    {
    public:
        Iterator(uintmem uL = 0) : m_uLocation(uL) {};

        Iterator(Iterator<T>&&) = default;
        Iterator(const Iterator<T>&) = default;

        ~Iterator() = default;

    public:
        Iterator<T>& operator++()
        {
            return m_uLocation += m_uJump;
        }

        Iterator<T> operator++()
        {
            auto tmp = m_uLocation;
            m_uLocation += m_uJump;
            return tmp;
        }

    private:
        uintmem m_uLocation;
        static const uintmem m_uJump = JumpOfSize<T>();
    };

    namespace Details
    {
        class BEE_API AllocatorImpl
        {
        public:
            AllocatorImpl() = delete;
            AllocatorImpl(uintmem);
            ~AllocatorImpl();

        protected:
            void  SetSize(uintmem);

            void* GetPtr() const { return m_pBlock; }

            const uintmem& GetCapacity() const { return m_uCapacity; }

            void  Resize(const uintmem&);

        private:
            void*   m_pBlock;
            void*   m_pTmp;
            uintmem m_uCapacity;
        };

        template <typename T>
        constexpr void DestroyAt(T* p)
        {
            p->~T();
        }
    }

    class BEE_API IAllocator 
        : public Details::AllocatorImpl
    {};

    template<
        class T, 
        uintmem min = 32, 
        uintmem growEvery = 4>
    class Allocator : 
        public Bee::Utils::Memory::IAllocator
    {
        uintmem m_uSize              = min;
        uintmem m_uResizeAmount      = min;
        uintmem m_uResizeAmountBytes = AllocatorImpl::GetCapacity();
        uintmem m_uResizeCounter     = 1;

    public:
        Allocator() : IAllocator(min * sizeof(T)) {};
        ~Allocator() = default;

    public:
// Getters --------------------------------------------------------------------
        using IAllocator::GetCapacity;

        using IAllocator::GetPtr;

        const uintmem& GetSize() const { return m_uSize; }

    public:
// Public Methods -------------------------------------------------------------
        void Resize()
        {
            if ((m_uResizeCounter++ % growEvery) == 0)
            {
                m_uResizeAmountBytes += m_uResizeAmountBytes;
                m_uResizeAmount      += m_uResizeAmount;
            }

            IAllocator::Resize(m_uResizeAmountBytes);
            m_uSize += m_uResizeAmount;
        }

    public:
// Operators ------------------------------------------------------------------
        T& operator[](const uintmem& uIndex) const
        {
            if (uIndex >= m_uSize)
                throw Problems::OutsideOfBuffer(BEE_COLLECT_DATA());

            return reinterpret_cast<T*>(this->GetPtr())[uIndex];
        }
    };
}
