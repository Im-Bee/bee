#pragma once

#include <cstdint>

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

namespace Bee::Utils::Memory
{
    typedef uint64_t uintmem;

    class BEE_API IAllocator
    {
    public:
        IAllocator()                    = default;
        ~IAllocator()                   = default;
        IAllocator(IAllocator&&)        = default;
        IAllocator(const IAllocator&)   = default;
    };

    namespace Details
    {
        class BEE_API AllocatorImpl
        {
            void*   m_Buffer;
            uintmem m_Capacity;

        public:
            AllocatorImpl() = delete;
            AllocatorImpl(uintmem);
            ~AllocatorImpl();

        public:
            void  Resize(const uintmem&);
            void  SetSize(uintmem);
            void* GetPtr() const { return m_Buffer; }
            const uintmem& GetCapacity() const { return m_Capacity; }
        };
    }

    template<
        class T, 
        uintmem min = 32, 
        uintmem growEvery = 4>
    class Allocator : 
        public Bee::Utils::Memory::IAllocator,
        private Details::AllocatorImpl
    {
        uintmem m_uSize        = min;
        uintmem m_uResize      = min;
        uintmem m_uResizeBytes = AllocatorImpl::GetCapacity();
        uintmem m_uResizeCount = 1;

    public:
        Allocator() : AllocatorImpl(min * sizeof(T)) {};
        ~Allocator() = default;

    public:
        using AllocatorImpl::GetCapacity;
        using AllocatorImpl::SetSize;

        uintmem GetSize() { return m_uSize; }

        void Resize()
        {
            if ((m_uResizeCount++ % growEvery) == 0)
            {
                m_uResizeBytes += m_uResizeBytes;
                m_uResize      += m_uResize;
            }

            AllocatorImpl::Resize(m_uResizeBytes);
            m_uSize += m_uResize;
        }

        T& operator[](const uintmem& uIndex) const
        {
            if (uIndex >= m_uSize)
                throw Problems::OutsideOfBuffer(BEE_COLLECT_DATA());

            return reinterpret_cast<T*>(this->GetPtr())[uIndex];
        }
    };

    template <typename T>
    constexpr void DestroyAt(T* p)
    {
        p->~T();
    }
}
