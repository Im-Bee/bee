#pragma once

#include "Types.h"

// void* operator new(usize, void* pPlace) noexcept
// {
//     return pPlace;
// }

namespace Duckers
{

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
inline T* PlaceAt(T* pPlace, TArgs&&... args) 
{
    return new (pPlace) T(::Duckers::Forward<TArgs>(args)...);
}

template<class T>
inline void DestroyAt(T& obj) 
{
    obj.~T();
}

} // !Duckers
