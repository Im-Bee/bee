#pragma once

#include "CoreMinimal.h"

namespace Duckers
{


template<class Type, typename Fn>
inline constexpr void ForEach(Type* pType, usize uSize, Fn&& lambda)
{
    while (uSize--) {
        lambda(pType[uSize]);
    }
}


} // !Duckers


