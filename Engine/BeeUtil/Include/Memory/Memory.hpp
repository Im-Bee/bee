#pragma once

#include <cstdint>

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

namespace Bee::Utils::Memory
{
    typedef uint64_t uintmem;

    template<int N>
    struct GetPowerOf2Exponent
    {
        static const int Value = 1 + GetPowerOf2Exponent< (N >> 1) >::Value;
    };

    template<>
    struct GetPowerOf2Exponent<1>
    {
        static const int Value = 0;
    };

    template<class T>
    class Iterator
    {
    public:
        Iterator(void* pLoc = nullptr) : m_uLocation(reinterpret_cast<uintmem>(pLoc)) {};
        explicit Iterator(uintmem uLoc) : m_uLocation(uLoc) {};

        Iterator(Iterator<T>&&) = default;
        Iterator(const Iterator<T>&) = default;

        ~Iterator() = default;

    public:
        Iterator<T> operator+(const Iterator<T>& other)
        {
            return Iterator<T>(this->m_uLocation + other.m_uLocation);
        }

        Iterator<T>&& operator+(Iterator<T>&& other)
        {
            other.m_uLocation += this->m_uLocation;
            return Move(other);
        }

        Iterator<T> operator-(const Iterator<T>& other)
        {
            return Iterator<T>(this->m_uLocation - other.m_uLocation);
        }

        Iterator<T>&& operator-(Iterator<T>&& other)
        {
            other.m_uLocation -= this->m_uLocation;
            return Move(other);
        }

        Iterator<T>& operator++()
        {
            this->m_uLocation += sizeof(T);
            return *this;
        }

        Iterator<T> operator++(int)
        {
            auto tmp = m_uLocation;
            m_uLocation += sizeof(T);
            return Iterator<T>(tmp);
        }

        Iterator<T>& operator--()
        {
            this->m_uLocation -= sizeof(T);
            return *this;
        }

        Iterator<T> operator--(int)
        {
            auto tmp = m_uLocation;
            m_uLocation -= sizeof(T);
            return Iterator<T>(tmp);
        }

        operator int()
        {
            return (m_uLocation >> GetPowerOf2Exponent<sizeof(T)>::Value);
        }

    private:
        uintmem m_uLocation;
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

            void* Get() const { return m_pBlock; }

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
        uintmem growEvery = 8>
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

        using IAllocator::Get;

        const uintmem& GetSize() const { return m_uSize; }

        Iterator<T> GetBegin() const { return Iterator<T>(this->Get()); }

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

            return reinterpret_cast<T*>(this->Get())[uIndex];
        }
    };
}
