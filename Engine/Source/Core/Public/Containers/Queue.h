#pragma once

#include "IAllocator.h"
#include "Utilites.h"



namespace Duckers
{


template<class Type, 
         class IAllocator = IAllocator<Type>,
         usize StartingCapacity = 32>
class Queue
{
    
    static const usize DefaultAllocationFlags = ENone;

public:

    Queue()
    { }

    ~Queue()
    { }

    Queue(Queue&&)
    { }

    Queue(const Queue&)
    { }

public:

    template<class U>
    void PushBack(U&&)
    { }

    void Pop()
    { }

public:

    usize GetCapacity() const 
    { return 0; }

    usize GetSize() const 
    { return 0; }

private:

private:

};


}
