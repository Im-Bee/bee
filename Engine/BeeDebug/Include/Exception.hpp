#pragma once

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

#define BEE_DECLARE_EXECPTION(exceptionName, msg)                               \
    class BEE_API exceptionName : public Bee::Problems::Exception               \
    {                                                                           \
    public:                                                                     \
        exceptionName() : Exception(msg) {};                                    \
        exceptionName(                                                          \
            const wchar_t* customMsg,                                           \
            CollectedData&& cd);                                                \
        explicit exceptionName(CollectedData&& cd);                             \
        ~exceptionName() = default;                                             \
    };

namespace Bee::Problems
{
#pragma region Exception configuration
    constexpr const int     LineNotCollected  = -1;
    constexpr const wchar_t NoReason[]	      = L"Unkonwn reason";
    constexpr const wchar_t NoFile[]          = L"Unkonwn file";

#   define BEE_NOT_IMPLEMENTED_MSG          L"[Not implemented]: Function/Method isn't fully implemented. "
#   define BEE_BAD_ALLOC_MSG                L"[Bad alloc]: Couldn't allocate data. "
#   define BEE_OUTSIDE_OF_BUFFER_MSG        L"[Outside of buffer]: Tried to get data outside of buffer's scope. "
#   define BEE_INVALID_ARGUMENT_MSG         L"[Invalid argument]: Passed argument was invalid. "
#   define BEE_PROBLEM_WITH_WIN_API_MSG     L"[Problem with WIN API]: WIN API didn't work as intended or there is an unhandled error. "
#   define BEE_NULLPTR_CALL_MSG             L"[Nullptr call]: Value was nullptr. "
#pragma endregion

    struct BEE_API CollectedData
    {
        CollectedData() = default;

        CollectedData(
            const wchar_t* szWhy,
            const wchar_t* szFile,
            int            line) 
        : szWhy(szWhy), 
          szFile(szFile), 
          Line(line) 
        {};

        CollectedData(
            const wchar_t* szFile,
            int            line)
        : szFile(szFile), 
          Line(line) 
        {};

        ~CollectedData() = default;

        const wchar_t* szWhy  = nullptr;
        const wchar_t* szFile = nullptr;
        const int      Line   = -1;
    };

    class BEE_API Exception
    {
    public:
                 Exception();
        explicit Exception(const wchar_t* szReason);
                 Exception(const wchar_t* szReason, CollectedData&& cd);
        
        ~Exception() = default;

    public:
        wchar_t const* const GetReason() const { return m_Collected.szWhy; }
        wchar_t const* const GetFile()   const { return m_Collected.szFile; }
        const int&           GetLine()   const { return m_Collected.Line; }

    private:
        void Dump() const;
        void PopUp() const;

    private:
        CollectedData m_Collected;
    };

    BEE_DECLARE_EXECPTION(NotImplemented,    BEE_NOT_IMPLEMENTED_MSG);
    BEE_DECLARE_EXECPTION(BadAlloc,          BEE_BAD_ALLOC_MSG);
    BEE_DECLARE_EXECPTION(OutsideOfBuffer,   BEE_OUTSIDE_OF_BUFFER_MSG);
    BEE_DECLARE_EXECPTION(InvalidArgument,   BEE_INVALID_ARGUMENT_MSG);
    BEE_DECLARE_EXECPTION(ProblemWithWINAPI, BEE_PROBLEM_WITH_WIN_API_MSG);
    BEE_DECLARE_EXECPTION(NullptrCall,       BEE_NULLPTR_CALL_MSG);
}
