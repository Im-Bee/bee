#pragma once



#define BEE_NOT_IMPLEMENTED_MSG       L"[Not implemented]: Function/Method isn't fully implemented."
#define BEE_BAD_ALLOC_MSG             L"[Bad alloc]: Couldn't allocate data."
#define BEE_OUTSIDE_OF_BUFFER_MSG     L"[Outside of buffer]: Tried to get data outside of buffer's scope."
#define BEE_INVALID_ARGUMENT_MSG      L"[Invalid argument]: Argument was invalid."
#define BEE_PROBLEM_WITH_WIN_API_MSG  L"[Problem with WIN API]: WIN API didn't work as anticipated or there was an unhandled error."
#define BEE_INVALID_PTR_MSG           L"[Invalid pointer]: Value was invalid or nullptr."



#define BEE_DECL_EXECPTION(name, msg)                                  \
    class BEE_API name : public ::Bee::Debug::Exception                \
    {                                                                  \
    public:                                                            \
        name() : Exception(msg) {};                                    \
        name(                                                          \
            const wchar_t* customMsg,                                  \
            ColletedOnException&& cd);                                 \
        explicit name(ColletedOnException&& cd);                       \
        ~name() = default;                                             \
    };                                                                 \



namespace Bee::Debug
{
    constexpr const int     LineNotCollected = -1;
    constexpr const wchar_t UnknownReason[]	 = L"Unkonwn reason";
    constexpr const wchar_t UnknownFile[]    = L"Unkonwn file";

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
                 Exception(const wchar_t* szReason, ColletedOnException&& cd);
        
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

    BEE_DECL_EXECPTION(NotImplemented,    BEE_NOT_IMPLEMENTED_MSG);
    BEE_DECL_EXECPTION(BadAlloc,          BEE_BAD_ALLOC_MSG);
    BEE_DECL_EXECPTION(OutsideOfBuffer,   BEE_OUTSIDE_OF_BUFFER_MSG);
    BEE_DECL_EXECPTION(InvalidArgument,   BEE_INVALID_ARGUMENT_MSG);
    BEE_DECL_EXECPTION(ProblemWithWINAPI, BEE_PROBLEM_WITH_WIN_API_MSG);
    BEE_DECL_EXECPTION(NullptrCall,       BEE_INVALID_PTR_MSG);
}
