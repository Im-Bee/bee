#pragma once

#include "Memory/Nodes.h"

namespace Duckers
{

template<class Type, typename Fn>
inline void ForEachNode(Node<Type>* pHead, Fn&& lambda)
{
    Node<Type>* pNext;

    while (pHead) 
    {
        pNext = pHead->pNext;
        lambda(pHead);
        pHead = pNext;
    }
}

} // !Duckers
