#pragma once

#include "AllocatorFlags.h"
#include "IAllocator.h"
#include "Allocs.h"

namespace Duckers
{


template<class Type, usize uPoolSize = 1024>
class DUCKERS_API PoolAllocator final : public Duckers::IAllocator<Type>
{

    using TypePtr = Type*;

    struct MemoryBlock
    {
        TypePtr pBuffer;

        MemoryBlock* pNext;
    };

    constexpr static const usize m_uPoolSizeBytes = uPoolSize * sizeof(Type);

public:

    PoolAllocator()
        : m_pMemoryBlocks(nullptr)
    { }

    ~PoolAllocator() 
    {
        InternalFreeMemory();
    }

    PoolAllocator(const PoolAllocator<Type>&) 
    {
        throw; // TODO: ...
    }

    PoolAllocator(PoolAllocator<Type>&& other)
        : m_pMemoryBlocks(other.m_pMemoryBlocks)
        , m_pLastBlock(other.m_pLastBlock)
    { 
        other.m_pMemoryBlocks = nullptr;
    }

public:

    TypePtr Allocate(usize uAmount, usize uFlags = 0)
    {
        if (uAmount > uPoolSize) {
            throw; // TODO: ...
        }

        TypePtr pAllocated;
        if (!(pAllocated = reinterpret_cast<TypePtr>(malloc(m_uPoolSizeBytes)))) {
            throw; // TODO: ...
        }

        HandleFlags(pAllocated, uFlags);

        TakeOwnershipOfBlock(pAllocated);

        return reinterpret_cast<Type*>(pAllocated);
    }

    void DeAllocate(Type* pAllocated, usize uDecontructAmount)
    { 
        if constexpr (CheckIsTrivial<Type>()) {
            return;
        }

        MemoryBlock* pMemBlock = m_pMemoryBlocks;
        MemoryBlock* pLastMemBlock = nullptr;

        while (pMemBlock) {
            if (pMemBlock->pBuffer != pAllocated) {
                pLastMemBlock = pMemBlock;
                pMemBlock = pMemBlock->pNext;
                continue;
            }

            if (uDecontructAmount > uPoolSize) {
                throw; // TODO: ...
            }

            if constexpr (!CheckIsTrivial<Type>()) {
                DeconstructData(pMemBlock->pBuffer, uDecontructAmount, uPoolSize);
            }

            if (pLastMemBlock) {
                pLastMemBlock->pNext = pMemBlock->pNext;
            } else {
                m_pMemoryBlocks = m_pMemoryBlocks->pNext;
            }

            free(pMemBlock->pBuffer);
            delete pMemBlock;

            return;
        }
        
        throw; // TODO: ...
    }

    TypePtr ReAllocate(Type*, usize, usize)  
    { 
        throw; // TODO: ...
    }

    constexpr static usize MinimumAlloc() 
    { return uPoolSize; }

private:

    void HandleFlags(void* pAllocated, usize uFlags)
    { 
        if (uFlags & ENone) {
            return;
        }

        if (uFlags | EZeroMemory) {
            ::Duckers::ZeroMemory(pAllocated, m_uPoolSizeBytes);
        }
    }

    void TakeOwnershipOfBlock(TypePtr memBlock) 
    {
        if (!m_pMemoryBlocks) {
            m_pLastBlock = m_pMemoryBlocks = new MemoryBlock { memBlock, nullptr };
            return;
        }
        
        m_pLastBlock = (m_pLastBlock->pNext = new MemoryBlock({ memBlock, nullptr }));
    }

    MemoryBlock* RetriveBlock(TypePtr memBlock)
    {
        MemoryBlock* pMemBlock = m_pMemoryBlocks;

        while (pMemBlock) {
            if (pMemBlock->pBuffer != memBlock) {
                pMemBlock = pMemBlock->pNext;
                continue;
            }

            return pMemBlock;
        }

        throw; // TODO: ...
    }

    void InternalFreeMemory()
    {
        MemoryBlock* pMemBlock = m_pMemoryBlocks;

        while (pMemBlock) {
            free(pMemBlock->pBuffer);

            auto* tmp = pMemBlock;
            pMemBlock = pMemBlock->pNext;
            delete tmp;
        }
    }

private:

    MemoryBlock* m_pMemoryBlocks;
    MemoryBlock* m_pLastBlock;

    MemoryBlock* m_pSharedBlocks;

};


} // !Duckers


