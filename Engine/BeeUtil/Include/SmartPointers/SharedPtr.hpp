#pragma once

namespace Bee::Utils
{
    template <typename T>
    class SharedBlock
    {
    public:

    };

    template <
        class T,
        class Block = SharedBlock<T>>
    class SharedPtr
    {
        Block* m_pObject;

    public:
        SharedPtr() : m_pObject(nullptr) {}
        SharedPtr(decltype(__nullptr)) : m_pObject(nullptr) {}
        SharedPtr(Block* pBlock) : m_pObject(pBlock) 
        {
            InternalAddRef();
        }

        ~SharedPtr()
        {
            InternalRelease();
        }

    protected:
        void InternalAddRef() const
        {
            
        }

        void InternalRelease() 
        {

        }
    };
}
