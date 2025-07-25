#pragma once

#include "AllocatorFlags.h"
#include "IAllocator.h"
#include "Allocs.h"

namespace Duckers
{


template<class Type>
class DUCKERS_API DynamicAllocator final : public Duckers::IAllocator<Type>
{

    using TypePtr = Type*;

    struct MemoryBlock
    {
        TypePtr pBuffer;
        usize uSize;

        MemoryBlock* pNext;
    };

public:

    DynamicAllocator()
        : m_pMemoryBlocks(nullptr)
    { }

    ~DynamicAllocator() 
    {
        InternalFreeMemory();
    }

    DynamicAllocator(const DynamicAllocator<Type>&) 
    {
        throw; // TODO: ...
    }

    DynamicAllocator(DynamicAllocator<Type>&& other)
        : m_pMemoryBlocks(other.m_pMemoryBlocks)
        , m_pLastBlock(other.m_pLastBlock)
    { 
        other.m_pMemoryBlocks = nullptr;
    }

public:

    TypePtr Allocate(usize uAmount, usize uFlags = 0)
    {
        TypePtr pAllocated;
        if (!(pAllocated = reinterpret_cast<TypePtr>(malloc(uAmount * sizeof(Type))))) {
            throw; // TODO: ...
        }

        HandleFlags(pAllocated, uAmount, uFlags);

        TakeOwnershipOfBlock(pAllocated, uAmount);

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

            if (uDecontructAmount > pMemBlock->uSize) {
                throw; // TODO: ...
            }

            if constexpr (!CheckIsTrivial<Type>()) {
                DeconstructData(pMemBlock->pBuffer, uDecontructAmount, pMemBlock->uSize);
            }

            if (uDecontructAmount < pMemBlock->uSize) {
                return;
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
    }

    TypePtr ReAllocate(Type* pAllocated, usize uOldSize, usize uNewSize)  
    { 
        MemoryBlock* pMemBlock = RetriveBlock(pAllocated);
    
        TypePtr pReAllocated;
        if constexpr (CheckIsTrivial<Type>()) {
            if (!(pReAllocated = reinterpret_cast<TypePtr>(realloc(pAllocated, uNewSize * sizeof(Type))))) {
                throw; // TODO: ...
            }
        }
        if constexpr (!CheckIsTrivial<Type>()) {
            if (!(pReAllocated = reinterpret_cast<TypePtr>(malloc(uNewSize * sizeof(Type))))) {
                throw; // TODO: ...
            }

            MoveData(pAllocated, uOldSize, pReAllocated);
            free(pAllocated);
        }

        pMemBlock->pBuffer = pReAllocated;
        pMemBlock->uSize = uNewSize;

        return pReAllocated;
    }

private:

    void HandleFlags(void* pAllocated, usize uAmount, usize uFlags)
    { 
        if (uFlags & ENone) {
            return;
        }

        if (uFlags | EZeroMemory) {
            ::Duckers::ZeroMemory(pAllocated, uAmount * sizeof(Type));
        }
    }

    void TakeOwnershipOfBlock(TypePtr memBlock, usize uSize) 
    {
        if (!m_pMemoryBlocks)
        {
            m_pLastBlock = m_pMemoryBlocks = new MemoryBlock { memBlock, uSize, nullptr };
            return;
        }
        
        m_pLastBlock = (m_pLastBlock->pNext = new MemoryBlock({ memBlock, uSize, nullptr }));
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

