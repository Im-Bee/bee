#pragma once

namespace Bee::Utils
{
    template <class T>
    class SharedBlock
    {
        template<class U, class Y>     friend class SharedPtr;
        template<class T, class... Ts> friend SharedBlock<T>& MakeShared(Ts&&...);

        using Instance = T;

    private:
        SharedBlock() 
        : m_Obj(), 
          m_uRefCount(0)
        {
            BEE_LOG(Debug::SmartPointers, 
                    L"SharedBlock (%p): Constructing", 
                    this);
        }

        template<class... Ts>
        SharedBlock(Ts... args) 
            : m_Obj(Move(args)...), 
            m_uRefCount(0)
        {
            BEE_LOG(
                Debug::SmartPointers,
                L"SharedBlock (%p): Constructing",
                this);
        }

        ~SharedBlock()
        {
            BEE_LOG(
                Debug::SmartPointers, 
                L"SharedBlock (%p): Deconstructing", 
                this);
        }

    private:
// Private Methods ------------------------------------------------------------
        void AddRef()
        {
            ++m_uRefCount;

            BEE_LOG(
                Debug::SmartPointers, 
                L"SharedBlock (%p): Adding ref (new count = %llu)", 
                this, 
                m_uRefCount);
        }

        void ReleaseRef()
        {
            if (m_uRefCount)
                --m_uRefCount;

            BEE_LOG(
                Debug::SmartPointers, 
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

    private:
        uint64_t m_uRefCount = -1;
        Instance m_Obj;
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

    public:
        SharedPtr(decltype(__nullptr)) : m_pObject(nullptr) {};
        
        SharedPtr(Block& block) : m_pObject(&block)
        {
            InternalAddRef();
        }

        SharedPtr(SharedPtr&& other) noexcept
            : m_pObject(Memory::Move(other.m_pObject))
        {
            other.m_pObject = nullptr;
        }

        SharedPtr(const SharedPtr& other) throw()
            : m_pObject(other.m_pObject)
        {
            InternalAddRef();
        }

        ~SharedPtr()
        {
            InternalRelease();
        }

    public:
// Public Methods -------------------------------------------------------------
        SharedType* Get()
        {
            if (!m_pObject)
            { 
                return nullptr;
            }

            return m_pObject->Ptr();
        }

    public:
// Operators ------------------------------------------------------------------
        SharedType* operator->() const
        {
            if (!m_pObject)
            {
                throw Debug::NullptrCall(BEE_COLLECT_DATA_ON_EXCEPTION());
            }

            return m_pObject->Ptr();
        }

        void operator=(Block& other)
        {
            this->InternalRelease();

            m_pObject = &other;
            this->InternalAddRef();
        }

        void operator=(const SharedPtr<T>& other)
        {
            this->InternalRelease();

            m_pObject = other.m_pObject;
            this->InternalAddRef();
        }

        void operator=(SharedPtr<T>&& other)
        {
            this->InternalRelease();

            m_pObject = Move(other.m_pObject);
            this->InternalAddRef();
        }

    protected:
// Private Methods ------------------------------------------------------------
        void InternalAddRef() const
        {
#ifdef _DEBUG
            if (m_pObject)
            {
                m_pObject->AddRef();
                BEE_LOG(
                    Debug::SmartPointers,
                    L"SharedPtr (%p): InternalAddRef() (Block %p)",
                    this,
                    m_pObject);
            }
            else
            {
                BEE_LOG(
                    Debug::Error,
                    L"SharedPtr (%p): InternalAddRef() called on nullptr",
                    this);
            }

            return;
#endif // _DEBUG

            m_pObject->AddRef();
        }

        void InternalRelease()
        {
#ifdef _DEBUG
            if (m_pObject)
            {
                m_pObject->ReleaseRef();
                BEE_LOG(
                    Debug::SmartPointers,
                    L"SharedPtr (%p): InternalRelease() (Block %p)",
                    this,
                    m_pObject);
            }

            return;
#endif // _DEBUG

            if (m_pObject)
                m_pObject->ReleaseRef();
        }

    private:
        Block* m_pObject = nullptr;
    };
#pragma warning(pop)

    template<
        class T,
        class... Ts>
    SharedBlock<T>& MakeShared(Ts&&... args)
    {
        return *(new SharedBlock<T>(Move(args)...));
    }
}
