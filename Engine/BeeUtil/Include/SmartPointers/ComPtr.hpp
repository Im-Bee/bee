#pragma once

namespace Bee::Utils
{
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
                L"ComPtr %p is initialized to nullptr",
                this);
        };
        ComPtr(decltype(__nullptr)) : m_pPtr(nullptr) 
        {
            B_LOG(
                Problems::SmartPointers,
                L"ComPtr %p is initialized to nullptr",
                this);
        }
        ComPtr(const ComPtr& other) throw() : m_pPtr(other.m_pPtr)
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
                    L"InternalAddRef() New count of ComPtr %p with Interface %p is %lu",
                    this,
                    m_pPtr,
                    count);
            }
            else
            {
                B_LOG(
                    Problems::Error,
                    L"InternalAddRef() called on ComPtr %p with nullptr",
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
                    L"InternalRelease() New count of ComPtr %p with Interface %p is %lu",
                    this,
                    m_pPtr,
                    count);

                m_pPtr = nullptr;
            }
            else
            {
                B_LOG(
                    Problems::Warning,
                    L"InternalRelease() called on ComPtr %p with nullptr",
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
}
