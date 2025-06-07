#pragma once

#include "Memory/Nodes.h"

namespace Duckers
{

template<class Type, typename Fn>
inline void ForEachNode(Node<Type>* pHead, Fn&& lambda)
{
    while (pHead) 
    {
        lambda(pHead);
        pHead = pHead->pNext;
    }
}

} // !Duckers
