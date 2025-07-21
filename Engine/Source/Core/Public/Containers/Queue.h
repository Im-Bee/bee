#pragma once

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
    using MemBlocks = Node<QueueNode*>;

    static const usize DefaultAllocationFlags = ENone;

public:

    Queue()
        : m_Allocator()
        , m_pMemBlocks(new MemBlocks(m_Allocator.Allocate(uPoolSize)))
        , m_pLastMemBlock(m_pMemBlocks)
        , m_uEnqueued(0)
        , m_pFirstInQueue(nullptr)
        , m_pLastEnqueued(nullptr)
    { }

    ~Queue()
    { }

    Queue(Queue&&)
    { }

    Queue(const Queue&)
    { }

public:

    template<class U>
    void PushBack(U&& item)
    { 
        if (m_uEnqueued >= uPoolSize) {
            m_pLastMemBlock = (m_pLastMemBlock->pNext = new MemBlocks(m_Allocator.Allocate(uPoolSize)));
        }
        
        if (!m_pFirstInQueue) {
            m_pFirstInQueue = m_pLastEnqueued = PlaceAt(&m_pLastMemBlock->Data[m_uEnqueued % uPoolSize], Forward<U>(item));
            return;
        }

        m_pLastEnqueued = (m_pLastEnqueued->pNext = PlaceAt(
                           &m_pLastMemBlock->Data[m_uEnqueued % uPoolSize], Forward<U>(item)));
    }

    Type& Pop()
    {
        return m_pFirstInQueue->Data;
    }

public:

    usize GetCapacity() const 
    { return 0; }

    usize GetSize() const 
    { return 0; }

private:

private:

    IAllocator m_Allocator;
    MemBlocks* m_pMemBlocks;
    MemBlocks* m_pLastMemBlock;

    usize m_uEnqueued;
    QueueNode* m_pFirstInQueue;
    QueueNode* m_pLastEnqueued;

};


}
