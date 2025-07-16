#pragma once 

#include "CoreMinimal.h"
#include "Utilites.h"

namespace Duckers
{


template<class Type>
inline constexpr bool CheckIsAligned(usize uSizeInBytes)
{
    return (uSizeInBytes % sizeof(Type)) == 0;
}


template<class Type>
struct IsTrivial
{ static constexpr bool Value = false; };

template<>
struct IsTrivial<bool>
{ static constexpr bool Value = true; };

template<>
struct IsTrivial<char>
{ static constexpr bool Value = true; };

template<>
struct IsTrivial<int8>
{ static constexpr bool Value = true; };

template<>
struct IsTrivial<int16>
{ static constexpr bool Value = true; };

template<>
struct IsTrivial<int32>
{ static constexpr bool Value = true; };

template<>
struct IsTrivial<int64>
{ static constexpr bool Value = true; };



template<class Type>
inline consteval bool CheckIsTrivial()
{
    return IsTrivial<Type>::Value; 
}


template<class Type>
inline constexpr usize Align(usize uSizeInBytes)
{
    return uSizeInBytes + (uSizeInBytes % sizeof(Type));
}


template<class Type>
inline void CopyFromTo(Type* pSource, usize uSourceSize, Type* pDest)
{
    while (uSourceSize--) {
        PlaceAt(&pDest[uSourceSize], pSource[uSourceSize]);
    }
}


template<class Type>
inline void MoveData(Type* pSource, usize uSourceSize, Type* pDest)
{
    while (uSourceSize--) {
        PlaceAt(&pDest[uSourceSize], Move(pSource[uSourceSize]));
    }
}


template<class Type>
inline void DeconstructData(Type* pSource, usize uSourceSize)
{
    for (usize i = 0; i < uSourceSize; ++i) {
        DestroyAt(pSource[i]);
    }
}


inline void ZeroMemory(void* pSource, usize uSizeInBytes)
{
    char* pBytes = reinterpret_cast<char*>(pSource);

    while (uSizeInBytes--) {
        pBytes[uSizeInBytes] = 0;
    }
}


} // !Duckers

