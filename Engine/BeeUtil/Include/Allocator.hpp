#pragma once

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

typedef uint64_t uintmem;

namespace Bee::Utils::Memory
{
    class BEE_API IAllocator
    {
    public:
        IAllocator() = default;
        ~IAllocator() = default;

        IAllocator(IAllocator&&) = default;
        IAllocator(const IAllocator&) = default;
    };

    namespace Impl
    {
        class BEE_API AllocatorImpl
        {
            void* m_Buffer;
            uintmem m_Capacity;

        public:
            AllocatorImpl() = delete;
            AllocatorImpl(const uintmem&);
            ~AllocatorImpl();

        public:
            void Resize(const uintmem&);
            void SetSize(const uintmem&);
            void* GetPtr() { return m_Buffer; }
            const uintmem& GetCapacity() { return m_Capacity; }
        };
    }

    template<
        class T, 
        uintmem min = 32, 
        uintmem growEvery = 4>
    class Allocator : 
        public Bee::Utils::Memory::IAllocator,
        private Impl::AllocatorImpl
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

        void Resize()
        {
            if ((m_uResizeCount++ % growEvery) == 0)
            {
                m_uResizeBytes += m_uResizeBytes;
                m_uResize += m_uResizeBytes;
            }

            AllocatorImpl::Resize(m_uResizeBytes);
            m_uSize += m_uResize;
        }

        T& operator[](const uintmem& uIndex)
        {
            if (uIndex >= m_uSize)
                throw Problems::OutsideOfBuffer(BEE_COLLECT_DATA());

            return reinterpret_cast<T*>(this->GetPtr())[uIndex];
        }
    };
}
