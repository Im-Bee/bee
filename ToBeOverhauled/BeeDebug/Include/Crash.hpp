#pragma once

namespace Bee::Debug
{
// ----------------------------------------------------------------------------
// * Interface of an object that is supposed to perform specific acitons before application
// shuts down after encountering an unhandled exception.
// 
// * Adds instance of itself to singleton of CrashHandler during construction and 
// removes itself on deconstruction.
// ----------------------------------------------------------------------------
    class BEE_API IOnException
    {
        friend class CrashHandler;

    public:
        IOnException();
        ~IOnException();

    protected:
        virtual void HandleObjects() = 0;
    };



#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class.
#pragma warning(disable : 4251)
// ----------------------------------------------------------------------------
// * Singleton that handles objects that are supposed to do something before
// application shuts down, because of unhandled exception.
// ----------------------------------------------------------------------------
    class BEE_API CrashHandler
    {
        friend class IOnException;
        friend class Exception;

        CrashHandler()  = default;
        ~CrashHandler() = default;

    public:
        CrashHandler(      CrashHandler&&) = delete;
        CrashHandler(const CrashHandler&)  = default;

        static CrashHandler& Get();

    private:
        void Attach(IOnException*);
        void Detach(IOnException*);
        void HandleObjects();

    private:
        static CrashHandler* m_pInstance;
    };
#pragma warning(pop)
}