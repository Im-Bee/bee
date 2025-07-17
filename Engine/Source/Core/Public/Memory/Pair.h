#pragma once

#include "Allocs.h"
#include "Utilites.h"



namespace Duckers
{


template<class TypeA, class TypeB>
class Pair
{
public:

    Pair() = default;
    
    template<typename U, typename Y>
    Pair(U&& a, Y&& b)
        : m_InstanceA(Forward<U>(a))
        , m_InstanceB(Forward<Y>(b))
    { }

    ~Pair() = default;

    Pair(Pair&& other)
    {
        if constexpr (!CheckIsTrivial<TypeA>) {
            this->m_InstanceA(Move(other.m_InstanceA));
        }
        if constexpr (!CheckIsTrivial<TypeB>) {
            this->m_InstanceB(Move(other.m_InstanceB));
        }
    }

    Pair(const Pair&) = default;

public:

    TypeA& GetA()
    { return m_InstanceA; }

    TypeA& GetB()
    { return m_InstanceB; }

private:

    TypeA m_InstanceA;
    TypeB m_InstanceB;

};


} // !Duckers


