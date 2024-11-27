#pragma once

namespace Bee::Utils
{
    template <class T>
    class SharedBlock
    {
        using Instance = T;

        uint64_t m_uRefCount = 0;
        Instance m_Obj;

    public:
        SharedBlock() 
            : m_Obj() 
        {
            BEE_LOG(
                Problems::SmartPointers, 
                L"SharedBlock (%p): Constructing", 
                this);
        }

        SharedBlock(T&& obj) 
            : m_Obj(Move(obj)) 
        {};

        ~SharedBlock()
        {
            BEE_LOG(
                Problems::SmartPointers, 
                L"SharedBlock (%p): Deconstructing", 
                this);
        }

    public:
        void AddRef()
        {
            ++m_uRefCount;

            BEE_LOG(
                Problems::SmartPointers, 
                L"SharedBlock (%p): Adding ref (new count = %llu)", 
                this, 
                m_uRefCount);
        }

        void ReleaseRef()
        {
            if (m_uRefCount)
                --m_uRefCount;

            BEE_LOG(
                Problems::SmartPointers, 
                L"SharedBlock (%p): Removing ref (new count = %llu)", 
                this, 
                m_uRefCount);

            if (!m_uRefCount)
                this->~SharedBlock();
        }

        Instance* Ptr()
        {
            return &m_Obj;
        }
    };

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    template <
        class T,
        class Block = SharedBlock<T>>
    class SharedPtr
    {
        template<class U, class Y> friend class SharedPtr;

        using SharedType = T;

        Block* m_pObject;

    public:
        SharedPtr() 
            : m_pObject(nullptr) 
        {
            BEE_LOG(
                Problems::SmartPointers,
                L"SharedPtr (%p): Constructing with nullptr", 
                this);
        }

        SharedPtr(decltype(__nullptr)) 
            : m_pObject(nullptr) 
        {
            BEE_LOG(
                Problems::SmartPointers, 
                L"SharedPtr (%p): Constructing with nullptr",
                this);
        }
        
        SharedPtr(SharedPtr&& other) 
            : m_pObject(Move(other.m_pObject))
        {}

        SharedPtr(const SharedPtr& other) 
            : m_pObject(other.m_pObject)
        {
            InternalAddRef();
        }

        ~SharedPtr()
        {
            BEE_LOG(
                Problems::SmartPointers, 
                L"SharedPtr (%p): Deconstructing", 
                this);

            InternalRelease();
        }

    protected:
        void InternalAddRef() const
        {
#ifdef _DEBUG
            if (m_pObject)
            {
                m_pObject->AddRef();
                BEE_LOG(
                    Problems::SmartPointers,
                    L"SharedPtr (%p): InternalAddRef() (Block %p)",
                    this,
                    m_pObject);
            }
            else
            {
                BEE_LOG(
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
                BEE_LOG(
                    Problems::SmartPointers,
                    L"SharedPtr (%p): InternalRelease() (Block %p)",
                    this,
                    m_pObject);
            }
            else
            {
                BEE_LOG(
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
                throw Problems::NullptrCall(BEE_COLLECT_DATA());

            return m_pObject->Ptr();
        }

        void operator=(Block* other)
        {
            if (m_pObject)
                this->InternalRelease();

            m_pObject = other;
            this->InternalAddRef();
        }

        void operator=(SharedPtr<T>&& other)
        {
            if (m_pObject)
                this->InternalRelease();

            m_pObject = Move(other.m_pObject);
            this->InternalAddRef();
        }

        operator bool() const
        {
            if (m_pObject)
                return true;
            else
                return false;
        }

    public:
        SharedType* GetPtr()
        {
            if (!m_pObject)
                throw Problems::NullptrCall(BEE_COLLECT_DATA());

            return m_pObject->Ptr();
        }
    };
#pragma warning(pop)

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
}
