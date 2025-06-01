#pragma once

#include "MemoryUtility.hpp"



template<typename Type, class Allocator = ::AISD::Allocators::GrowingAllocator<Type, 16, 16>>
class Stack 
{
public:
    constexpr Stack() : m_pAllocator(new Allocator()), m_uInUse(0)
    {
        m_pAllocator->Alloc(); 
    }

    Stack(Stack<Type>&& other) : m_pAllocator(Move(other.m_pAllocator)), m_uInUse(other.m_uInUse)
    {
    }

    Stack(const Stack<Type>& other) : m_pAllocator(new Allocator(*reinterpret_cast<Allocator*>(other.m_pAllocator))), 
    m_uInUse(other.m_uInUse)
    {
    }
    
    ~Stack() 
    {
        while (m_uInUse > 0) {
            DestroyAt(m_pAllocator->Get(--m_uInUse));
        }
    
        if (m_pAllocator) {
            m_pAllocator->Free();
            delete m_pAllocator;
        }
    }

public:
    Stack<Type, Allocator>& operator=(Stack<Type, Allocator>&& other)
    {
        this->~Stack();
        
        m_pAllocator = other.m_pAllocator;
        m_uInUse = other.m_uInUse;

        other.m_pAllocator = nullptr;
        other.m_uInUse = 0;

        return *this;
    }

public:
    size_t GetSize() const { return m_uInUse; }

    size_t GetCapacity() const { return m_pAllocator->GetSize(); } 
 
    bool IsEmpty() const { return IsEmptyInternal(); }

public:
    template<typename U>
    void Push(U&& item)
    {
        if (m_uInUse >= m_pAllocator->GetSize()) {
            m_pAllocator->Resize();
        }

        PlaceAt(&(m_pAllocator->Get(m_uInUse++)), Forward<U>(item));
    }

    const Type& Peek() 
    {
        if (IsEmptyInternal()) {
            throw std::runtime_error("Peek() called on empty stack");
        }

        return m_pAllocator->Get(m_uInUse - 1);
    }

    Type Pop() 
    {
        if (IsEmptyInternal()) {
            throw std::runtime_error("Pop() called on empty stack");
        }


        Type* pElem = &(m_pAllocator->Get(--m_uInUse));
        Type value = Move(*pElem);

        DestroyAt(*pElem);
        return value;
    }



private:
    bool IsEmptyInternal() const { return m_uInUse == 0; }

private:
    ::AISD::Allocators::IAllocator<Type>* m_pAllocator = nullptr;
    size_t m_uInUse = -1;

};
