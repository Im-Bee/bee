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
    using MemBlockNode = Node<QueueNode*>;

    static const usize DefaultAllocationFlags = ENone;

public:

    Queue()
        : m_Allocator()
        , m_pMemBlocks(new MemBlockNode(m_Allocator.Allocate(uPoolSize)))
        , m_pLastMemBlock(m_pMemBlocks)
        , m_uEnqueued(0)
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
        if (m_uEnqueued >= uPoolSize) {
            m_pLastMemBlock = (m_pLastMemBlock->pNext = new MemBlockNode(m_Allocator.Allocate(uPoolSize)));
        }
        
        if (!m_pFirstInQueue) {
            m_pFirstInQueue = m_pLastEnqueued = PlaceAt(&m_pLastMemBlock->Data[m_uEnqueued % uPoolSize], 
                                                        Forward<U>(item));

            ++m_uEnqueued;
            return;
        }

        m_pLastEnqueued = (m_pLastEnqueued->pNext = PlaceAt(
                           &m_pLastMemBlock->Data[m_uEnqueued % uPoolSize], Forward<U>(item)));
        ++m_uEnqueued;
    }

    Type& Pop()
    {
        return m_pFirstInQueue->Data;
    }

public:

    usize GetCapacity() const 
    { return m_uEnqueued + (uPoolSize - m_uEnqueued); }

    usize GetSize() const 
    { return m_uEnqueued; }

private:

    void InternalReleaseMemBlockNodes()
    {
        MemBlockNode* toBeDeleted;
        while (m_pMemBlocks) {
            toBeDeleted = m_pMemBlocks;
            m_pMemBlocks = m_pMemBlocks->pNext;

            free(toBeDeleted);
        }
    }

private:

    IAllocator m_Allocator;
    MemBlockNode* m_pMemBlocks;
    MemBlockNode* m_pLastMemBlock;

    usize m_uEnqueued;
    QueueNode* m_pFirstInQueue;
    QueueNode* m_pLastEnqueued;

};


}
