#pragma once

namespace Bee::Utils::Memory
{
    typedef long long          b_isize;
    typedef unsigned long long b_usize;

    template<class T>
    struct RemoveRef { using Type = T; };

    template<class T>
    struct RemoveRef<T&> { using Type = T; };

    template<class T>
    struct RemoveRef<T&&> { using Type = T; };

    template<class T>
    constexpr typename RemoveRef<T>::Type&& Move(T&& arg) noexcept
    {
        return static_cast<RemoveRef<T>::Type&&>(arg);
    }

    BEE_API inline void InMemoryObjMove(void* pDest, void* pSource, const b_usize& uSourceSize);

    template<typename T>
    constexpr T& MoveOnConstruct(T* pMem, T&& obj)
    {
        InMemoryObjMove(pMem, &obj, sizeof(T));
        return *pMem;
    }

    template<int N>
    struct GetPowerOf2Exponent
    {
        static constexpr const int Value = 1 + GetPowerOf2Exponent< (N >> 1) >::Value;
    };

    template<>
    struct GetPowerOf2Exponent<1>
    {
        static constexpr const int Value = 0;
    };
    
    template<class T>
    class Iterator
    {
    public:
        Iterator(void* pLoc = nullptr) : m_uAddInt(reinterpret_cast<b_usize>(pLoc)) {};
        explicit Iterator(b_usize uLoc) : m_uAddInt(uLoc) {};

        Iterator(Iterator<T>&&) = default;
        Iterator(const Iterator<T>&) = default;

        ~Iterator() = default;

    public:
        T& Ref()
        {
            return *reinterpret_cast<T*>(m_uAddInt);
        }

    public:
        Iterator<T> operator+(const Iterator<T>& other)
        {
            return Iterator<T>(this->m_uAddInt + other.m_uAddInt);
        }

        Iterator<T> operator+(const b_usize& num)
        {
            return Iterator<T>(this->m_uAddInt + (num << GetPowerOf2Exponent<sizeof(T)>::Value));
        }

        Iterator<T> operator-(const Iterator<T>& other)
        {
            return Iterator<T>(this->m_uAddInt - other.m_uAddInt);
        }

        Iterator<T>&& operator-(Iterator<T>&& other)
        {
            other.m_uAddInt -= this->m_uAddInt;
            return Move(other);
        }

        Iterator<T>& operator++()
        {
            this->m_uAddInt += sizeof(T);
            return *this;
        }

        Iterator<T> operator++(int)
        {
            auto tmp = m_uAddInt;
            m_uAddInt += sizeof(T);
            return Iterator<T>(tmp);
        }

        Iterator<T>& operator--()
        {
            this->m_uAddInt -= sizeof(T);
            return *this;
        }

        Iterator<T> operator--(int)
        {
            auto tmp = m_uAddInt;
            m_uAddInt -= sizeof(T);
            return Iterator<T>(tmp);
        }

        T* operator->()
        {
            return reinterpret_cast<T*>(m_uAddInt);
        }

        operator int()
        {
            return static_cast<int>(m_uAddInt >> GetPowerOf2Exponent<sizeof(T)>::Value);
        }

    private:
        b_usize m_uAddInt = -1;
    };

    namespace Details
    {
        class BEE_API AllocatorImpl
        {
        public:
            AllocatorImpl() = delete;
            AllocatorImpl(b_usize);
            ~AllocatorImpl();

        protected:
            void  SetSize(b_usize);

            void* Get() const { return m_pBlock; }

            const b_usize& GetCapacity() const { return m_uCapacity; }

            void  Resize(const b_usize&);

        private:
            void*     m_pBlock    = nullptr;
            void*     m_pTmp      = nullptr;
            b_usize   m_uCapacity = -1;
        };

        template <typename T>
        constexpr void DestroyAt(T* p)
        {
            p->~T();
        }
    }

    class BEE_API IAllocator : public Details::AllocatorImpl
    {};

    template<class T, 
             b_usize min = 32, 
             b_usize growEvery = 8>
    class Allocator : public Bee::Utils::Memory::IAllocator
    {
        b_usize m_uSize              = min;
        b_usize m_uResizeAmount      = min;
        b_usize m_uResizeAmountBytes = AllocatorImpl::GetCapacity();
        b_usize m_uResizeCounter     = 1;

    public:
        Allocator() 
        : IAllocator(min * sizeof(T)) 
        {};

        ~Allocator() = default;

// Getters --------------------------------------------------------------------
    public:
        using IAllocator::GetCapacity;

        using IAllocator::Get;

        const b_usize&    GetSize() const { return m_uSize; }

              Iterator<T> GetBegin() const { return Iterator<T>(this->Get()); }

// Setters --------------------------------------------------------------------
    public:
        using IAllocator::SetSize;

// Public Methods -------------------------------------------------------------
    public:
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

// Operators ------------------------------------------------------------------
    public:
        T& operator[](const b_usize& uIndex) const
        {
            if (uIndex >= this->GetCapacity())
            {
                throw ::Bee::Debug::OutsideOfBuffer(BEE_COLLECT_DATA_ON_EXCEPTION());
            }

            return reinterpret_cast<T*>(this->Get())[uIndex];
        }
    };
}
