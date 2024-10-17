#pragma once

namespace Bee::Utils
{
#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    template <typename T>
    class ComPtr
    {
        typedef T InterfaceType;
        
        InterfaceType* m_pPtr;

    public:
        ComPtr() : m_pPtr(nullptr) 
        {
            B_LOG(
                Problems::SmartPointers,
                L"ComPtr (%p): Constructing with nullptr",
                this);
        };
        ComPtr(decltype(__nullptr)) : m_pPtr(nullptr) 
        {
            B_LOG(
                Problems::SmartPointers,
                L"ComPtr (%p): Constructing with nullptr",
                this);
        }

        ComPtr(ComPtr&&) = default;

        ComPtr(const ComPtr& other) throw() :
            m_pPtr(other.m_pPtr)
        {
            InternalAddRef();
        }

        ~ComPtr() throw()
        {
            InternalRelease();
        }

    protected:
        template<class U> friend class ComPtr;
        
        void InternalAddRef() const
        {
#ifdef _DEBUG
            if (m_pPtr)
            {
                ULONG count;
                count = m_pPtr->AddRef();
                B_LOG(
                    Problems::SmartPointers,
                    L"ComPtr (%p): InternalAddRef() New count of interface %p is %lu",
                    this,
                    m_pPtr,
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

            if (m_pPtr)
                m_pPtr->AddRef();
        }

        void InternalRelease()
        {
#ifdef _DEBUG
            if (m_pPtr)
            {
                ULONG count;
                count = m_pPtr->Release();
                B_LOG(
                    Problems::SmartPointers,
                    L"ComPtr (%p): InternalRelease() New count of interface %p is %lu",
                    this,
                    m_pPtr,
                    count);

                m_pPtr = nullptr;
            }
            else
            {
                B_LOG(
                    Problems::Warning,
                    L"ComPtr (%p): InternalRelease() with nullptr",
                    this);
            }
            
            return;
#endif // _DEBUG

            if (m_pPtr)
            {
                m_pPtr->Release();
                m_pPtr = nullptr;
            }
        }

    public:
        InterfaceType* operator->() const
        {
            if (!m_pPtr)
                throw Problems::CallOnNullptr(B_COLLECT_DATA());

            return m_pPtr;
        }
        
        InterfaceType** operator&()
        {
            return &m_pPtr;
        }

        template<class U>
        void operator=(const ComPtr<U>& other)
        {
            if (!m_pPtr)
            {
                m_pPtr = other.m_pPtr;
                this->InternalAddRef();
                return;
            }

            this->InternalRelease();
            m_pPtr = other.m_pPtr;
            this->InternalAddRef();
        }

    public:
        void Swap(ComPtr&& other)
        {
            InterfaceType* tmp = m_pPtr;
            this->m_pPtr = other.m_pPtr;
            other.m_pPtr = tmp;
        }

        InterfaceType* Get() const
        {
            return m_pPtr;
        }
    };
#pragma warning(pop)
}
