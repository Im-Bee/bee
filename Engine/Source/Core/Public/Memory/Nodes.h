#pragma once

#include "Memory/Utilites.h"
#include "ApiExportImport.h"


namespace Duckers
{

template<class Type>
struct DUCKERS_API Node
{
    Type Data;
    Node<Type>* pNext;

    Node() 
        : Data(nullptr)
        , pNext(nullptr)
    { }

    explicit Node(decltype(nullptr)) 
        : Data(nullptr)
        , pNext(nullptr)
    { }
    
    template<class U>
    explicit Node(U&& constructed) 
        : Data(::Duckers::Forward<U>(constructed))
        , pNext(nullptr)
    { }

    ~Node() = default;
};

} // !Duckers
