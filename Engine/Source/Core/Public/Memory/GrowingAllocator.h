#pragma once

#include "IAllocator.h"

namespace Duckers
{


template<class Type>
class DUCKERS_API GrowingAllocator final : public ::Duckers::IAllocator<Type>
{

    using TypePtr = Type*;

public:

    GrowingAllocator()
        : m_uSizeInBytes(0)
        , m_uAmountOfData(0)
    { Allocate(0); }

    ~GrowingAllocator()
    {  DeAllocate(); }

    GrowingAllocator(const IAllocator<Type>& other)
    {
        this->Allocate(other.GetSizeInBytes());
        // TODO: ...
    }

    GrowingAllocator(IAllocator<Type>&& other)
    {
        this->m_pBuffer = other.Leak();
        // TODO: ...
    }

public:

    void Allocate(usize uSizeInBytes) override final
    { 
        if (m_uSizeInBytes >= uSizeInBytes) {
            return;
        }
    }

    void DeAllocate() override final
    { 
        if (!m_pBuffer) {
            return;
        }
    }

    void ReSize() override final
    { }

    TypePtr Leak() override final
    { 
        TypePtr pLeak = m_pBuffer;
        m_pBuffer = nullptr;

        return pLeak;
    }

    inline Type& Get(usize uIndex) override final
    { 
        if (uIndex >= m_uAmountOfData) {
            throw; // TODO: ...
        }

        return m_pBuffer[uIndex]; 
    }

    inline usize GetSizeInBytes() override final
    { return m_uSizeInBytes; }

private:



private:

    TypePtr m_pBuffer;
    
    usize m_uSizeInBytes;
    usize m_uAmountOfData;

};


} // !Duckers

