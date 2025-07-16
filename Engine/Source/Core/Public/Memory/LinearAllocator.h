#pragma once

#include "AllocatorFlags.h"
#include "Arrays.h"
#include "IAllocator.h"
#include "Allocs.h"

namespace Duckers
{


template<class Type>
class DUCKERS_API LinearAllocator final : public Duckers::IAllocator<Type>
{

    using TypePtr = Type*;

    struct MemoryBlock
    {
        TypePtr pBuffer;
        usize uSize;

        MemoryBlock* pNext;
    };

public:

    LinearAllocator()
        : m_pMemoryBlocks({ 0, 0, 0 })
    { }

    ~LinearAllocator() 
    { 
        MemoryBlock* pMemBlock = m_pMemoryBlocks.pNext;

        while (pMemBlock) {
            free(pMemBlock->pBuffer);

            auto* tmp = pMemBlock;
            pMemBlock = pMemBlock->pNext;
            free(tmp);
        }

        free(m_pMemoryBlocks.pBuffer);
    }

    LinearAllocator(const LinearAllocator<Type>&) 
    {
        throw; // TODO: ...
    }

    LinearAllocator(LinearAllocator<Type>&&) 
    { 
        throw; // TODO: ...
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

    void DeAllocate(Type* pAllocated, usize uUsed)
    { 
        if constexpr (CheckIsTrivial<Type>()) {
            return;
        }

        MemoryBlock* pMemBlock = &m_pMemoryBlocks;

        while (pMemBlock) {
            if (pMemBlock->pBuffer != pAllocated) {
                pMemBlock = pMemBlock->pNext;
                continue;
            }

            if (uUsed > pMemBlock->uSize) {
                throw; // TODO: ...
            }

            DeconstructData(pMemBlock->pBuffer, uUsed);

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
        if (m_pMemoryBlocks.pBuffer == nullptr)
        {
            m_pLastBlock = &(m_pMemoryBlocks = MemoryBlock({ memBlock, uSize, nullptr }));
            return;
        }
        
        m_pLastBlock = (m_pLastBlock->pNext = new MemoryBlock({ memBlock, uSize, nullptr }));
    }

    MemoryBlock* RetriveBlock(TypePtr memBlock)
    {
        MemoryBlock* pMemBlock = &m_pMemoryBlocks;

        while (pMemBlock) {
            if (pMemBlock->pBuffer != memBlock) {
                pMemBlock = pMemBlock->pNext;
                continue;
            }

            return pMemBlock;
        }

        throw; // TODO: ...
    }

private:

    MemoryBlock m_pMemoryBlocks;
    MemoryBlock* m_pLastBlock;

};


} // !Duckers

