#pragma once

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

#include <vector>

namespace Bee::Problems
{
    class Exception;

    class BEE_API IDumpOnException
    {
    public:
        virtual void Dump() = 0;
    };

#pragma warning(push)
    // Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API CrashHandling
    {
        using DumpVector = std::vector<IDumpOnException*>;

        friend Exception;

        CrashHandling() = default;

    public:
        ~CrashHandling() = default;

        CrashHandling(const CrashHandling&) = delete;
        CrashHandling(CrashHandling&&) = delete;

        static CrashHandling& Get();

    public:
        void ProtectObject(IDumpOnException* obj);

    private:
        void Dump();

    private:
        static CrashHandling* m_pInstance;
        
        DumpVector m_vpObjects = {};
    };
#pragma warning(pop)
}