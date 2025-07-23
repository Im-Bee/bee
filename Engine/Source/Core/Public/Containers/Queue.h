#pragma once

#include "Allocs.h"
#include "Nodes.h"
#include "PoolAllocator.h"
#include "Utilites.h"



namespace Duckers
{


template<class Type, 
         usize uPoolSize = 16,
         class IAllocator = PoolAllocator<Type, uPoolSize>>
class Queue
{
    using MemBlockNode = Node<Type*>;

    static const usize DefaultAllocationFlags = ENone;

public:

    Queue()
        : m_Allocator()
        , m_pMemBlocks(new MemBlockNode(m_Allocator.Allocate(uPoolSize)))
        , m_pLastMemBlock(m_pMemBlocks)
        , m_uCapacity(uPoolSize)
        , m_uEnqueued(0)
        , m_uDequeued(0)
    { }

    ~Queue()
    { 
        ReleaseMemBlockNodes();
    }

    Queue(Queue&&)
    { 
        throw; // TODO: ...
    }

    Queue(const Queue&)
    { 
        throw; // TODO: ...
    }

public:

    template<class U>
    void PushBack(U&& item)
    {
        const usize uIndexPos = CalculateIndexPos();

        if (uIndexPos == CalculateOnWhatIndexReSize()) {
            m_pLastMemBlock = (m_pLastMemBlock->pNext 
                    = new MemBlockNode(m_Allocator.Allocate(uPoolSize)));
            CalculateCapacity();
        }
        
        PlaceAt(&m_pLastMemBlock->Data[uIndexPos], Forward<U>(item));

        ++m_uEnqueued;
    }

    Type Pop()
    {
        Type result = Move(m_pMemBlocks->Data[m_uDequeued % uPoolSize]);

        ++m_uDequeued;

        if (m_uDequeued % uPoolSize == 0) {
            ReleaseFirstUnusedMemBlock();
        }

        return result;
    }

public:

    usize GetCapacity() const
    { return m_uCapacity; }

    usize GetSize() const 
    { 
        isize diff = static_cast<isize>(m_uEnqueued) - m_uDequeued; 
        return diff < 0 ? -diff : diff;
    }

private:

    void ReleaseMemBlockNodes()
    {
        MemBlockNode* pToBeDeletedMemBlock;

        while (m_pMemBlocks) 
        {
            pToBeDeletedMemBlock = m_pMemBlocks;
            m_pMemBlocks = m_pMemBlocks->pNext;
            delete pToBeDeletedMemBlock;
            
            if constexpr (CheckIsTrivial<Type>()) {
                continue;
            }

            for (usize i = 0; i < uPoolSize; ++i) {
                m_pMemBlocks->Data[i].~Type();
            }
        }
    }

    void CalculateCapacity()
    {
        m_uCapacity += uPoolSize;
    }

    usize CalculateIndexPos()
    {
        return (m_uEnqueued - m_uDequeued) % uPoolSize;
    }

    constexpr usize CalculateOnWhatIndexReSize() 
    {
        return uPoolSize - 1;
    }

    void ReleaseFirstUnusedMemBlock()
    {
        if (!m_pMemBlocks->pNext) {
            return;
        }

        auto* pNext = m_pMemBlocks->pNext;
        m_Allocator.DeAllocate(m_pMemBlocks->Data, 0);

        delete m_pMemBlocks;

        m_pMemBlocks = pNext;
    }

private:

    IAllocator m_Allocator;
    MemBlockNode* m_pMemBlocks;
    MemBlockNode* m_pLastMemBlock;

    usize m_uCapacity;

    usize m_uEnqueued;
    usize m_uDequeued;

};


}
