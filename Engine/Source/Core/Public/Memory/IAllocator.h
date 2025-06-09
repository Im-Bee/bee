#pragma once

#include "CoreMinimal.h"
#include "ApiExportImport.h"


namespace Duckers
{


template<class Type>
class DUCKERS_API IAllocator
{
public:

    virtual void Allocate() = 0;

    virtual void DeAllocate() = 0;

    virtual void ReSize() = 0;

    virtual Type* Leak() = 0;

    inline virtual Type* Get(usize) = 0;

};


} // !Duckers
