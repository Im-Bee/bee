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
        using StoredClass = T;

        uint64_t    m_uRefCount = 0;
        StoredClass m_Obj;

    public:
        SharedBlock() : m_Obj() 
        {
            B_LOG(Problems::SmartPointers, L"SharedBlock (%p): Constructing", this);
        }

        SharedBlock(T&& obj) : m_Obj(Move(obj)) {};

        ~SharedBlock()
        {
            B_LOG(Problems::SmartPointers, L"SharedBlock (%p): Deconstructing", this);
        }

    public:
        void AddRef()
        {
            ++m_uRefCount;

            B_LOG(Problems::SmartPointers, L"SharedBlock (%p): Adding ref (new count = %llu)", this, m_uRefCount);
        }

        void ReleaseRef()
        {
            if (m_uRefCount)
                --m_uRefCount;

            B_LOG(Problems::SmartPointers, L"SharedBlock (%p): Removing ref (new count = %llu)", this, m_uRefCount);

            if (!m_uRefCount)
                this->~SharedBlock();
        }

        StoredClass* Ptr()
        {
            return &m_Obj;
        }
    };

    template<class T>
    SharedBlock<T>* MakeShared()
    {
        auto p = new SharedBlock<T>();
        p->AddRef();
        return p;
    }

    template<class T>
    SharedBlock<T>* MakeShared(T&& obj)
    {
        return new SharedBlock<T>(Move(obj));
    }

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    template <
        class T,
        class Block = SharedBlock<T>>
    class SharedPtr
    {
        using SharedType = T;

        Block* m_pObject;

    public:
        SharedPtr() : m_pObject(nullptr) 
        {
            B_LOG(Problems::SmartPointers, L"SharedPtr (%p): Constructing with nullptr", this);
        }

        SharedPtr(decltype(__nullptr)) : m_pObject(nullptr) 
        {
            B_LOG(Problems::SmartPointers, L"SharedPtr (%p): Constructing with nullptr", this);
        }
        
        SharedPtr(SharedPtr&& other)
            : m_pObject(other.m_pObject)
        {};

        SharedPtr(const SharedPtr& other) :
            m_pObject(other.m_pObject)
        {
            InternalAddRef();
        }

        ~SharedPtr()
        {
            B_LOG(Problems::SmartPointers, L"SharedPtr (%p): Deconstructing", this);

            InternalRelease();
        }

    protected:
        template<class U> friend class ComPtr;

        void InternalAddRef() const
        {
#ifdef _DEBUG
            if (m_pObject)
            {
                m_pObject->AddRef();
                B_LOG(
                    Problems::SmartPointers,
                    L"SharedPtr (%p): InternalAddRef() (Interface %p)",
                    this,
                    m_pObject->Ptr());
            }
            else
            {
                B_LOG(
                    Problems::Error,
                    L"SharedPtr (%p): InternalAddRef() called on nullptr",
                    this);
            }

            return;
#endif // _DEBUG

            if (m_pObject)
                m_pObject->AddRef();
        }

        void InternalRelease() 
        {
#ifdef _DEBUG
            if (m_pObject)
            {
                m_pObject->ReleaseRef();
                B_LOG(
                    Problems::SmartPointers,
                    L"SharedPtr (%p): InternalRelease() (Interface %p)",
                    this,
                    m_pObject->Ptr());
            }
            else
            {
                B_LOG(
                    Problems::Error,
                    L"SharedPtr (%p): InternalRelease() called on nullptr",
                    this);
            }

            return;
#endif // _DEBUG

            if (m_pObject)
                m_pObject->ReleaseRef();
        }

    public:
        SharedType* operator->() const
        {
            if (!m_pObject)
                throw Problems::CallOnNullptr(B_COLLECT_DATA());

            return m_pObject->Ptr();
        }

        void operator=(Block* other)
        {
            if (m_pObject)
                this->InternalRelease();

            m_pObject = other;
        }

        template<class U>
        void operator=(const SharedPtr<U>& other)
        {
            if (!m_pObject)
            {
                m_pObject = other.m_pObject;
                this->InternalAddRef();
                return;
            }

            this->InternalRelease();
            m_pObject = other.m_pObject;
            this->InternalAddRef();
        }

    public:
        SharedType* GetPtr()
        {
            if (!m_pObject)
                throw Problems::CallOnNullptr(B_COLLECT_DATA());

            return m_pObject->Ptr();
        }
    };
#pragma warning(pop)
}
