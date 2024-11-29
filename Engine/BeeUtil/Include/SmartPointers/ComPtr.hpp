#pragma once

namespace Bee::Utils
{
#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    template <class T>
    class ComPtr
    {
        template<class U> friend class ComPtr;

        typedef T Interface;

    public:
// Constructors ---------------------------------------------------------------
        ComPtr(decltype(__nullptr)) : m_pObject(nullptr) {};
        
        ComPtr(ComPtr&& other) noexcept 
            : m_pObject(Move(other.m_pObject)) 
        {
            other.m_pObject = nullptr;
        }

        ComPtr(const ComPtr& other) throw() 
            : m_pObject(other.m_pObject)
        {
            InternalAddRef();
        }

        ~ComPtr() 
        {
            InternalRelease();
        }

    public:
// Public Methods -------------------------------------------------------------
        void Swap(ComPtr&& other)
        {
            Interface* tmp = m_pObject;
            this->m_pObject = other.m_pObject;
            other.m_pObject = tmp;
        }

        Interface* Get() const
        {
            return m_pObject;
        }

    public:
// Operators ------------------------------------------------------------------
        Interface* operator->() const
        {
            if (!m_pObject)
                throw Problems::NullptrCall(BEE_COLLECT_DATA());

            return m_pObject;
        }
        
        Interface** operator&()
        {
            return &m_pObject;
        }

        operator Interface*()
        {
            return this->Get();
        }

        void operator=(const ComPtr<T>& other)
        {
            this->InternalRelease();

            m_pObject = other.m_pObject;
            this->InternalAddRef();
        }

        void operator=(ComPtr<T>&& other)
        {
            this->InternalRelease();

            m_pObject = Move(other.m_pObject);
            other.m_pObject = nullptr;
        }

        template<class U>
        void operator=(ComPtr<U>&& other)
        {
            this->InternalRelease();

            m_pObject = Move(other.m_pObject);
            other.m_pObject = nullptr;
        }

        template<class U>
        void operator=(const ComPtr<U>& other)
        {
            this->InternalRelease();

            m_pObject = other.m_pObject;
            this->InternalAddRef();
        }

    protected:
        // Private Methods ------------------------------------------------------------
        void InternalAddRef() const
        {
#ifdef _DEBUG
            if (m_pObject)
            {
                BEE_LOG(
                    Problems::SmartPointers,
                    L"ComPtr (%p): InternalAddRef() New count of interface %p is %lu",
                    this,
                    m_pObject,
                    m_pObject->AddRef());
            }
            else
            {
                BEE_LOG(
                    Problems::Error,
                    L"ComPtr (%p): InternalAddRef() with nullptr",
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
                BEE_LOG(
                    Problems::SmartPointers,
                    L"ComPtr (%p): InternalRelease() New count of interface %p is %lu",
                    this,
                    m_pObject,
                    m_pObject->Release());

                m_pObject = nullptr;
            }

            return;
#endif // _DEBUG

            if (m_pObject)
            {
                m_pObject->Release();
                m_pObject = nullptr;
            }
        }

    private:
        Interface* m_pObject = nullptr;

    };
#pragma warning(pop)
}
