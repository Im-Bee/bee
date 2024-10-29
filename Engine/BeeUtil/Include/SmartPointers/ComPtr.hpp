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
        
        Interface* m_pObject;

    public:
        ComPtr() 
            : m_pObject(nullptr) 
        {
            B_LOG(
                Problems::SmartPointers,
                L"ComPtr (%p): Constructing with nullptr",
                this);
        };
        
        ComPtr(decltype(__nullptr)) 
            : m_pObject(nullptr) 
        {
            B_LOG(
                Problems::SmartPointers,
                L"ComPtr (%p): Constructing with nullptr",
                this);
        }
        
        ComPtr(ComPtr&& other) 
            : m_pObject(Move(other.m_pObject))
        {}

        ComPtr(const ComPtr& other) throw() 
            : m_pObject(other.m_pObject)
        {
            InternalAddRef();
        }

        ~ComPtr() throw()
        {
            InternalRelease();
        }

    protected:
        void InternalAddRef() const
        {
#ifdef _DEBUG
            if (m_pObject)
            {
                ULONG count = m_pObject->AddRef();
                B_LOG(
                    Problems::SmartPointers,
                    L"ComPtr (%p): InternalAddRef() New count of interface %p is %lu",
                    this,
                    m_pObject,
                    count);
            }
            else
            {
                B_LOG(
                    Problems::Error,
                    L"ComPtr (%p): InternalAddRef() with nullptr",
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
                ULONG count = m_pObject->Release();
                B_LOG(
                    Problems::SmartPointers,
                    L"ComPtr (%p): InternalRelease() New count of interface %p is %lu",
                    this,
                    m_pObject,
                    count);

                m_pObject = nullptr;
            }
            else
            {
                B_LOG(
                    Problems::SmartPointers,
                    L"ComPtr (%p): InternalRelease() with nullptr",
                    this);
            }
            
            return;
#endif // _DEBUG

            if (m_pObject)
            {
                m_pObject->Release();
                m_pObject = nullptr;
            }
        }

    public:
        Interface* operator->() const
        {
            if (!m_pObject)
                throw Problems::CallOnNullptr(B_COLLECT_DATA());

            return m_pObject;
        }
        
        Interface** operator&()
        {
            return &m_pObject;
        }

        void operator=(const ComPtr<T>& other)
        {
            if (m_pObject)
                this->InternalRelease();

            m_pObject = other.m_pObject;
            this->InternalAddRef();
        }

        void operator=(ComPtr<T>&& other)
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

        template<class U>
        void operator=(const ComPtr<U>& other)
        {
            if (m_pObject)
            {
                this->InternalRelease();
            }
            m_pObject = other.m_pObject;
            this->InternalAddRef();
        }

    public:
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
    };
#pragma warning(pop)
}
