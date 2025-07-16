#pragma once

#include "IAllocator.h"
#include "LinearAllocator.h"
#include "Utilites.h"



namespace Duckers
{


template<class Type, class IAllocator = LinearAllocator<Type>>
class Vector
{
    
    static const usize MaxGrowth = 1 << 16;

public:

    Vector()
        : m_uContains(0)
        , m_uGrowth(32)
        , m_uCapacity(m_uGrowth)
        , m_Allocator()
        , m_pBuffer(m_Allocator.Allocate(m_uCapacity, ENone))
    { }

    ~Vector()
    { 
        m_Allocator.DeAllocate(m_pBuffer, m_uContains);
    }

public:

    template<class U>
    void PushBack(U&& item)
    {
        if (m_uContains >= m_uCapacity) {
            m_pBuffer = m_Allocator.ReAllocate(m_pBuffer, m_uCapacity, m_uCapacity + m_uGrowth);
            m_uCapacity += m_uGrowth;

            ReCalcGrowth();
        }

        PlaceAt(&m_pBuffer[m_uContains++], Forward<U>(item));
    }

    void Pop()
    {
        m_Allocator.DeAllocate(m_pBuffer, 1);
    }

public:

    usize GetCapacity() const 
    { return m_uCapacity; }

    usize GetSize() const 
    { return m_uContains; }

private:

    void ReCalcGrowth()
    {
        if (m_uGrowth < MaxGrowth) {
            m_uGrowth = m_uGrowth << 1;
        }
    }

private:

    usize m_uContains;

    usize m_uGrowth;
    usize m_uCapacity;

    IAllocator m_Allocator;
    Type* m_pBuffer;

};


}
