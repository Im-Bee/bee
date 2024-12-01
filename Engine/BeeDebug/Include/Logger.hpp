#pragma once

#ifndef BEE_API
#	define BEE_API 
#endif // !BEE_API

namespace Bee::Problems
{
    enum Severity
    {
        None            = 0x00,
        Bee             = (None + 1),
        Info            = (Bee + 1),
        Warning         = (Info + 1),
        Error           = (Warning + 1),
        SmartPointers   = (Error + 1),
        Allocators      = (SmartPointers + 1),
        DirectX         = (Allocators + 1),
    };

#pragma warning(push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning(disable : 4251)
    class BEE_API Logger
    {
        Logger();

    public:
        ~Logger()
#ifdef _DEBUG
            throw()
#endif // _DEBUG
            ;

        Logger(const Logger&) = delete;
        Logger(Logger&&)      = delete;

        static Logger& Get();

    public:
        void SetPath(const wchar_t* szPath);
        
        template<size_t ArrSize>
        void SetIgnore(Severity (&list)[ArrSize]);

    public:
        void Log(Severity&& sev, const wchar_t* format, ...);

    private:
        void Work();

    private:
        class _Impl;
        _Impl* m_pImpl;

    private:
        const wchar_t*  m_szTargetFile;
        Severity*       m_pIgnoreList;
        size_t          m_uIgnoreListSize;

        static Logger*  m_pInstance;
    };
#pragma warning(pop)

    template<size_t ArrSize>
    inline void Logger::SetIgnore(Severity (&list)[ArrSize])
    {
        m_pIgnoreList = list;
        m_uIgnoreListSize = ArrSize;
    }
}