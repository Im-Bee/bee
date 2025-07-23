#pragma once

#include "Allocs.h"
#include "Nodes.h"
#include "PoolAllocator.h"
#include "Utilites.h"



namespace Duckers
{


template<class Type, 
         usize uPoolSize = 128,
         class QueueNode = Node<Type>,
         class IAllocator = PoolAllocator<QueueNode, uPoolSize>>
class Queue
{
    using MemBlockNode = Node<QueueNode*>;

    static const usize DefaultAllocationFlags = ENone;

public:

    Queue()
        : m_Allocator()
        , m_pMemBlocks(new MemBlockNode(m_Allocator.Allocate(uPoolSize)))
        , m_pLastMemBlock(m_pMemBlocks)
        , m_uCapacity(uPoolSize)
        , m_uEnqueued(0)
        , m_uDequeued(0)
        , m_pFirstInQueue(nullptr)
        , m_pLastEnqueued(nullptr)
    { }

    ~Queue()
    { 
        InternalReleaseMemBlockNodes();
    }

    Queue(Queue&&)
    { }

    Queue(const Queue&)
    { }

public:

    template<class U>
    void PushBack(U&& item)
    {
        const usize uIndexPos = InternalCalcIndexPos();

        if (uIndexPos == InternalCalcOnWhatIndexReSize()) {
            m_pLastMemBlock = (m_pLastMemBlock->pNext 
                    = new MemBlockNode(m_Allocator.Allocate(uPoolSize)));
            InternalCalculateCapacity();
        }
        
        if (!m_pFirstInQueue) {
            m_pFirstInQueue = m_pLastEnqueued = PlaceAt(&m_pLastMemBlock->Data[uIndexPos], 
                                                        Forward<U>(item));
        } else {
            m_pLastEnqueued = (m_pLastEnqueued->pNext = PlaceAt(&m_pLastMemBlock->Data[uIndexPos],
                                                                Forward<U>(item)));
        }

        ++m_uEnqueued;
    }

    Type Pop()
    {
        Type result = Move(m_pFirstInQueue->Data);
        QueueNode* pNext = m_pFirstInQueue->pNext;

        m_pFirstInQueue->~QueueNode();
        m_pFirstInQueue = pNext;

        ++m_uDequeued;

        if (m_uDequeued % uPoolSize == 0) 
        {
            auto* pNext = m_pMemBlocks->pNext;
            m_Allocator.DeAllocate(m_pMemBlocks->Data, 0);
            m_pMemBlocks->Data = nullptr;
            delete m_pMemBlocks;
            m_pMemBlocks = pNext;
        }

        return result;
    }

public:

    usize GetCapacity() const
    { return m_uCapacity; }

    usize GetSize() const 
    { 
        isize diff = static_cast<isize>(m_uEnqueued) - m_uDequeued; 
        return diff < 0 ? 0 : diff;
    }

private:

    void InternalReleaseMemBlockNodes()
    {
        MemBlockNode* pToBeDeletedMemBlock;

        while (m_pMemBlocks) {
            pToBeDeletedMemBlock = m_pMemBlocks;
            m_pMemBlocks = m_pMemBlocks->pNext;
            delete pToBeDeletedMemBlock;
            
            if constexpr (CheckIsTrivial<Type>()) {
                continue;
            }

            for (usize i = 0; i < uPoolSize; ++i) {
                m_pMemBlocks->Data[i].Data.~Type();
            }
        }
    }

    void InternalCalculateCapacity()
    {
        m_uCapacity += uPoolSize;
    }

    usize InternalCalcIndexPos()
    {
        return (m_uEnqueued - m_uDequeued) % uPoolSize;
    }

    constexpr usize InternalCalcOnWhatIndexReSize() 
    {
        return uPoolSize - 1;
    }

private:

    IAllocator m_Allocator;
    MemBlockNode* m_pMemBlocks;
    MemBlockNode* m_pLastMemBlock;

    usize m_uCapacity;

    usize m_uEnqueued;
    usize m_uDequeued;
    QueueNode* m_pFirstInQueue;
    QueueNode* m_pLastEnqueued;

};


}
