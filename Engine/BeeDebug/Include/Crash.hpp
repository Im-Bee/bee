#pragma once

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

#include <list>

namespace Bee::Problems
{

// ----------------------------------------------------------------------------
// Interface for an object that is supposed to dump data before application 
// throws. Adds itself to CrashHandling instance during construction and 
// removes itself on deconstruction.
// 
//                               IDumpOnException
// ----------------------------------------------------------------------------
    class BEE_API IDumpOnException
    {
        friend class CrashHandling;

    public:
        IDumpOnException();
        ~IDumpOnException();

    protected:
        virtual void Dump() = 0;

    };

// ----------------------------------------------------------------------------
// Singleton that handles objects that are supposed to do something before
// application throws an exception
// 
//                               CrashHandling
// ----------------------------------------------------------------------------
#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API CrashHandling
    {
        using ListOfObjects = std::list<IDumpOnException*>;

        friend class IDumpOnException;
        friend class Exception;

        CrashHandling()  = default;
        ~CrashHandling() = default;

    public:
        CrashHandling(const CrashHandling&) = delete;
        CrashHandling(CrashHandling&&)      = delete;

        static CrashHandling& Get();

    private:
        void AddToList(IDumpOnException*);
        void Remove(IDumpOnException*);
        void Dump();

    private:
        static CrashHandling* m_pInstance;
        
        ListOfObjects m_vpObjects = ListOfObjects();
    };
#pragma warning(pop)
}