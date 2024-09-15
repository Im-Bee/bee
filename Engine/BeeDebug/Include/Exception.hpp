#pragma once

#ifndef BEE_API
#   define BEE_API 
#endif // !BEE_API

#define BEE_COLLECT_DATA() (Bee::Problems::CollectedData(BEE_AS_WCHAR __FILE__, __LINE__))

namespace Bee::Problems
{
#pragma region Exception configuration
    constexpr int     MaxPath			= 255;
    constexpr int     LineNotCollected  = -1;
    constexpr wchar_t NoReason[]	    = L"Unkonwn reason";
    // constexpr int  NoReasonSize		= sizeof(NoReason);
    constexpr wchar_t NoFile[]          = L"Unkonwn file";
    // constexpr int  NoFileSize        = sizeof(NoFile);

#define BEE_DECLARE_EXECPTION(exceptionName, msg)                           \
class BEE_API exceptionName : public Bee::Problems::Exception               \
{                                                                           \
public:                                                                     \
    exceptionName() : Exception(msg) {};                                    \
    exceptionName(                                                          \
                  const wchar_t* customMsg,                                 \
                  CollectedData&& cd);                                      \
    exceptionName(CollectedData&& cd);                                      \
    ~exceptionName() = default;                                             \
};

#define BEE_NOT_IMPLEMENTED_MSG  L"[Not implemented]: Function/Method isn't fully implemented. "
#define BEE_OUTSIDE_OF_BUFFER_MSG L"[Outside of buffer]: Tried to get data outside of buffer's scope. "
#define BEE_INVALID_ARGUMENT_MSG  L"[Invalid argument]: Passed argument was invalid. "
#define BEE_PROBLEM_WITH_WIN_API_MSG  L"[Problem with WIN API]: WIN API didn't work as intended or there is an unhandled error. "
#define BEE_CALL_ON_NULLPTR_MSG  L"[Call on nullptr]: Value was nullptr. "
#pragma endregion

    struct BEE_API CollectedData
    {
        CollectedData() = default;
        CollectedData(
            const wchar_t* szWhy,
            const wchar_t* szFile,
            int line) 
            : szWhy(szWhy), szFile(szFile), Line(line) {};
        CollectedData(
            const wchar_t* szFile,
            int line) 
            : szFile(szFile), Line(line) {};

        ~CollectedData() = default;

        const wchar_t* szWhy  = nullptr;
        const wchar_t* szFile = nullptr;
        const int      Line   = 0;
    };

    class BEE_API Exception
    {
        Bee::Problems::CollectedData m_Collected;

    public:
        Exception();
        explicit Exception(const wchar_t* szReason);
        Exception(const wchar_t* szReason, CollectedData&& cd);
        ~Exception() = default;

    public:
        const wchar_t* GetReason()  { return m_Collected.szWhy; }
        const wchar_t* GetFile()    { return m_Collected.szFile; }
        const int&  GetLine()       { return m_Collected.Line; }

    };

    BEE_DECLARE_EXECPTION(NotImplemented, BEE_NOT_IMPLEMENTED_MSG);
    BEE_DECLARE_EXECPTION(OutsideOfBuffer, BEE_OUTSIDE_OF_BUFFER_MSG);
    BEE_DECLARE_EXECPTION(InvalidArgument, BEE_INVALID_ARGUMENT_MSG);
    BEE_DECLARE_EXECPTION(ProblemWithWINAPI, BEE_PROBLEM_WITH_WIN_API_MSG);
    BEE_DECLARE_EXECPTION(CallOnNullptr, BEE_CALL_ON_NULLPTR_MSG);
}
