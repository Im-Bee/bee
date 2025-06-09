#pragma once

#include "IAllocator.h"

namespace Duckers
{


template<class Type>
class DUCKERS_API GrowingAllocator final : public ::Duckers::IAllocator<Type>
{
public:

    void Allocate() override final
    { }

    void DeAllocate() override final
    { }

    void ReSize() override final
    { }

    Type* Leak() override final
    { return nullptr; }

    inline Type* Get(usize) override final
    { return nullptr; }

};


} // !Duckers

