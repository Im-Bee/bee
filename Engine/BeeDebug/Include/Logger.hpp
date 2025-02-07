#pragma once

#ifndef BEE_API
#	define BEE_API 
#endif // !BEE_API

namespace Bee::Debug
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

// Constructors ---------------------------------------------------------------
    public:
        ~Logger();

        Logger(      Logger&&) = delete;
        Logger(const Logger&)  = delete;

        static Logger& Get();

// Setters --------------------------------------------------------------------
    public:
        void SetPath(const wchar_t* szPath);
        
        template<size_t uArrSize>
        void SetIgnoredTags(Severity (&pSeverityList)[uArrSize]);

        void SetIgnoredTags(Severity* pSeverityList, const size_t& uArrSize);

// Public Methods -------------------------------------------------------------
    public:
        void Log(Severity&& sev, const wchar_t* format, ...);

// Private --------------------------------------------------------------------
    private:
        void Loop();

    private:
        class _Impl;
        _Impl* m_pImpl = nullptr;

        const wchar_t*  m_szTargetFile    = nullptr;
        Severity*       m_pIgnoreList     = nullptr;
        size_t          m_uIgnoreListSize = -1;

        static Logger*  m_pInstance;
    };
#pragma warning(pop)

    template<size_t uArrSize>
    inline void Logger::SetIgnoredTags(Severity (&pSeverityList)[uArrSize])
    {
        return Logger::SetIgnoredTags(pSeverityList, uArrSize);
    }
}