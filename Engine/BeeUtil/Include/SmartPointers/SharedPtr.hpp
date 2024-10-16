#pragma once

namespace Bee::Utils
{
    template <class T>
    struct RemoveRef { using Type = T; };
    
    template <class T>
    struct RemoveRef<T&> { using Type = T; };

    template <class T>
    struct RemoveRef<T&&> { using Type = T; };

    template <class T>
    constexpr RemoveRef<T>::Type&& Move(T&& arg) noexcept
    {
        return static_cast<RemoveRef<T>::Type&&>(arg);
    }

    template <class T>
    class SharedBlock
    {
        uint64_t m_uRefs = 0;
        T m_Obj;

    public:
        SharedBlock() : m_Obj() 
        {
            B_LOG(Problems::SmartPointers, L"Allocating block for %p SharedBlock", this);
        }

        SharedBlock(T&& obj) : m_Obj(Move(obj)) {};

        ~SharedBlock()
        {
            B_LOG(Problems::SmartPointers, L"DeAllocating block for %p SharedBlock", this);
        }

    public:
        void AddRef()
        {
            ++m_uRefs;

            B_LOG(Problems::SmartPointers, L"Adding ref to %p SharedBlock, new count %llu", this, m_uRefs);
        }

        void ReleaseRef()
        {
            if (m_uRefs)
                --m_uRefs;

            B_LOG(Problems::SmartPointers, L"Removing ref to %p SharedBlock, new count %llu", this, m_uRefs);

            if (!m_uRefs)
                this->~SharedBlock();
        }

        T* Ptr() const
        {
            return &m_Obj;
        }
    };

    template<class T>
    SharedBlock<T>* MakeShared()
    {
        return new SharedBlock<T>();
    }

    template<class T>
    SharedBlock<T>* MakeShared(T&& obj)
    {
        return new SharedBlock<T>(Move(obj));
    }

    template <
        class T,
        class Block = SharedBlock<T>>
    class SharedPtr
    {
        using BlockType = T;

        Block* m_pObject;

    public:
        SharedPtr() : m_pObject(nullptr) {}
        SharedPtr(decltype(__nullptr)) : m_pObject(nullptr) {}
        SharedPtr(Block* pBlock) : m_pObject(pBlock) 
        {
            InternalAddRef();
        }

        SharedPtr(const SharedPtr& other)
        {
            if (m_pObject)
                this->InternalRelease();

            m_pObject = other.m_pObject;
            this->InternalAddRef();
        }

        ~SharedPtr()
        {
            InternalRelease();
        }

    protected:
        void InternalAddRef() const
        {
            m_pObject->AddRef();
        }

        void InternalRelease() 
        {
            m_pObject->ReleaseRef();
        }

    public:
        BlockType* operator->() const
        {
            return m_pObject->Ptr();
        }
    };
}
