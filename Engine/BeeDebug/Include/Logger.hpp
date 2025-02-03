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
        ~Logger()
#ifdef _DEBUG
            throw()
#endif // _DEBUG
            ;

        Logger(      Logger&&) = delete;
        Logger(const Logger&)  = delete;

        static Logger& Get();

// Setters --------------------------------------------------------------------
    public:
        void SetPath(const wchar_t* szPath);
        
        template<size_t ArrSize>
        void SetIgnoredTags(Severity (&list)[ArrSize]);

// Public Methods -------------------------------------------------------------
    public:
        void Log(Severity&& sev, const wchar_t* format, ...);

// Private --------------------------------------------------------------------
    private:
        void Loop();

    private:
        class _Impl;
        _Impl* m_pImpl;

        const wchar_t*  m_szTargetFile;
        Severity*       m_pIgnoreList;
        size_t          m_uIgnoreListSize;

        static Logger*  m_pInstance;
    };
#pragma warning(pop)

    template<size_t ArrSize>
    inline void Logger::SetIgnoredTags(Severity (&list)[ArrSize])
    {
        m_pIgnoreList = new Severity[ArrSize];
        for (int i = 0; i < ArrSize; ++i)
        {
            m_pIgnoreList[i] = list[i];
        }

        m_uIgnoreListSize = ArrSize;
    }
}