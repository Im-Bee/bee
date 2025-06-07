#pragma once

#define BEE_DECL_EXECPTION(name, msg)                                  \
    class BEE_API name : public ::Bee::Debug::Exception                \
    {                                                                  \
    public:                                                            \
        name() : Exception(msg) {};                                    \
                                                                       \
        name(const wchar_t* customMsg,                                 \
             ColletedOnException&& cd)                                 \
        : ::Bee::Debug::Exception(customMsg, cd)		               \
        {}															   \
                                                                       \
        explicit name(ColletedOnException&& cd)                        \
        : ::Bee::Debug::Exception(msg, cd)			                   \
        {}                                                             \
                                                                       \
        ~name() = default;                                             \
    };                                                                 \

namespace Bee::Debug
{
    struct BEE_API ColletedOnException
    {
        ColletedOnException() = default;

        ColletedOnException(const wchar_t* szWhy,
                            const wchar_t* szFile,
                                  int      line) 
        : szWhy(szWhy), 
          szFile(szFile), 
          Line(line) 
        {};

        ColletedOnException(const wchar_t* szFile,
                                  int      line)
        : szFile(szFile), 
          Line(line) 
        {};

        ~ColletedOnException() = default;

        const wchar_t* szWhy  = nullptr;
        const wchar_t* szFile = nullptr;
        const int      Line   = -1;
    };

    class BEE_API Exception
    {
    public:
                 Exception();
        explicit Exception(const wchar_t* szReason);
                 Exception(const wchar_t* szReason, ColletedOnException cd);
        
        ~Exception() = default;

    public:
        wchar_t const* const GetReason() const { return m_Collected.szWhy; }
        wchar_t const* const GetFile()   const { return m_Collected.szFile; }
        const int&           GetLine()   const { return m_Collected.Line; }

    private:
        void OnShutdown() const;
        void PopUp() const;

    private:
        ColletedOnException m_Collected;
    };

    BEE_DECL_EXECPTION(NotImplemented,
                       L"[Not implemented]: Function/Method isn't fully implemented.");

    BEE_DECL_EXECPTION(BadAlloc,          
                       L"[Bad alloc]: Couldn't allocate data.");

    BEE_DECL_EXECPTION(OutsideOfBuffer,  
                       L"[Outside of buffer]: Tried to get data outside of buffer's scope.");

    BEE_DECL_EXECPTION(InvalidArgument,   
                       L"[Invalid argument]: Argument was invalid.");

    BEE_DECL_EXECPTION(ProblemWithWINAPI, 
                       L"[Problem with WIN API]: WIN API didn't work as anticipated or there was an unhandled error.");

    BEE_DECL_EXECPTION(NullptrCall,       
                       L"[Invalid pointer]: Value was invalid or nullptr.");
}
