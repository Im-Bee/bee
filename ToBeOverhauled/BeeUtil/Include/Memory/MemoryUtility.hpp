#pragma once

#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <type_traits>



template<class T> struct RemoveRef { using Type = T; };

template<class T> struct RemoveRef<T&> { using Type = T; };

template<class T> struct RemoveRef<T&&> { using Type = T; };

template<class T>
inline typename RemoveRef<T>::Type&& Move(T&& arg) noexcept
{
    return static_cast<typename RemoveRef<T>::Type&&>(arg);
}

template<typename T>
inline T&& Forward(typename RemoveRef<T>::Type& arg) noexcept 
{
    return static_cast<T&&>(arg);
}

template<typename T>
inline T&& Forward(typename RemoveRef<T>::Type&& arg) noexcept 
{
    return static_cast<T&&>(arg);
}

template<class T, class... TArgs>
inline void PlaceAt(T* pPlace, TArgs&&... args) 
{
    new (pPlace) T(::AISD::Forward<TArgs>(args)...);
}

template<class T>
inline void DestroyAt(T& pObj) 
{
    pObj.~T();
}

template<class T>
inline void CopyAndPlace(T* pFrom, T* pTo, size_t uAmount)
{
    for (--uAmount; uAmount != SIZE_MAX; --uAmount) {
        PlaceAt(&pTo[uAmount], pFrom[uAmount]);
    }
}

template<class T>
inline void MoveAndPlace(T* pFrom, T* pTo, size_t uAmount)
{
    for (--uAmount; uAmount != SIZE_MAX; --uAmount) {
        PlaceAt(&pTo[uAmount], ::AISD::Move(pFrom[uAmount]));
    }
}

template<class T>
inline void DestroyWholeArray(T& pObj, size_t uAmount) 
{
    for (--uAmount; uAmount != SIZE_MAX; --uAmount) {
        DestroyAt(pObj[uAmount]);
    }
}





namespace Allocators 
{

template<class Type>
class IAllocator
{
public:
    virtual ~IAllocator() {};
    
public:
    virtual size_t GetSize() const = 0;

    virtual Type* GetBegin() const = 0;

    virtual Type& Get(size_t) const = 0;

public:
    virtual void Alloc() = 0;

    virtual void Resize() = 0;

    virtual void Free() = 0;

};


template<class Type, size_t uStartingSize = 16, size_t uIncreaseReAllocSizePer = 4>
class GrowingAllocator : public IAllocator<Type>
{
public:
    GrowingAllocator() 
        : m_uReAllocAmount(uStartingSize),
        m_uReAllocBytesAmount(sizeof(Type) * uStartingSize),
        m_uReAllocations(0),  
        m_uObjectsAmount(uStartingSize),
        m_uBytesBufferSize(sizeof(Type) * uStartingSize) 
    {
    }

    virtual ~GrowingAllocator()
    {
        Free();
    }

    GrowingAllocator(GrowingAllocator&& other)
        : m_uReAllocAmount(other.m_uReAllocAmount),
        m_pBuffer(other.m_pBuffer),  
        m_uReAllocBytesAmount(other.m_uReAllocBytesAmount),
        m_uReAllocations(other.m_uReAllocations),  
        m_uObjectsAmount(other.m_uObjectsAmount),
        m_uBytesBufferSize(other.m_uBytesBufferSize) 
    {
        other.m_pBuffer = nullptr;
        other.m_uObjectsAmount = 0;
        other.m_uBytesBufferSize = 0;
    }

    GrowingAllocator(const GrowingAllocator& other) 
        : m_uReAllocAmount(other.m_uReAllocAmount),
        m_uReAllocBytesAmount(sizeof(Type) * uStartingSize),
        m_uReAllocations(other.m_uReAllocations),  
        m_uObjectsAmount(other.m_uObjectsAmount),
        m_uBytesBufferSize(other.m_uBytesBufferSize) 
    {
        Alloc();
        CopyAndPlace(other.m_pBuffer, this->m_pBuffer, this->m_uObjectsAmount);
    }

public:
    size_t GetSize() const override final { return m_uObjectsAmount; }

    Type* GetBegin() const override final { return m_pBuffer; }
    
    Type& Get(size_t uIndex) const override final { return m_pBuffer[uIndex]; }

public:
    void Alloc() override final
    {
        Type* pNewAlloc;
        if (!(pNewAlloc = reinterpret_cast<Type*>(malloc(m_uBytesBufferSize)))) {
            throw std::bad_alloc();
        }
        
        m_pBuffer = pNewAlloc;
    }

    constexpr void Resize() override final
    {
        if (m_uReAllocations++ % uIncreaseReAllocSizePer == 0) {
            IncreaseNextReAllocsSize();
        }
        
        ReAllocInternal();
        IncreaseAvaliableSpace();
    }

    void Free() override final
    {
        if (!m_pBuffer) {
            return;
        }

        free(m_pBuffer);

        m_pBuffer = nullptr;
        m_uBytesBufferSize = 0;
    }

public:
    Type& operator[](size_t uIndex)
    {
        return m_pBuffer[uIndex];
    }

private:
    void IncreaseNextReAllocsSize() 
    {
        m_uReAllocBytesAmount = m_uReAllocBytesAmount << 1;
        m_uReAllocAmount = m_uReAllocAmount << 1;
    }

    void IncreaseAvaliableSpace() 
    {
        m_uObjectsAmount += m_uReAllocAmount;
        m_uBytesBufferSize += m_uReAllocBytesAmount;
    }

    void ReAllocInternalTrivial()
    {
        if (!(m_pBuffer = reinterpret_cast<Type*>(
                                realloc(m_pBuffer, 
                                        m_uBytesBufferSize + m_uReAllocBytesAmount)))) 
        {
            throw std::bad_alloc();
        }
    }

    void ReAllocInternalNonTrivial()
    {
        Type* newAlloc;
        if (!(newAlloc = reinterpret_cast<Type*>(malloc(m_uBytesBufferSize + m_uReAllocBytesAmount)))) 
        {
            throw std::bad_alloc();
        }

        MoveAndPlace(m_pBuffer, newAlloc, m_uObjectsAmount);
        free(m_pBuffer);
        m_pBuffer = newAlloc;
    }
    
    constexpr void ReAllocInternal() 
    {
        if constexpr (std::is_trivial_v<Type>) {
            ReAllocInternalTrivial();
        } else {
            ReAllocInternalNonTrivial();
        }

    }


private:
    Type* m_pBuffer = nullptr;

    size_t m_uReAllocAmount = -1;
    size_t m_uReAllocBytesAmount = -1;

    size_t m_uReAllocations = -1;

    size_t m_uObjectsAmount = -1;
    size_t m_uBytesBufferSize = -1;
};

} // !Allocators
