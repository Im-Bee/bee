#pragma once

// ----------------------------------------------------------------------------
// System headers
// ----------------------------------------------------------------------------

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN
#	include <Windows.h>

#   define BEE_WIN32
#endif // _WIN32



// ----------------------------------------------------------------------------
// API import/export macros
// ----------------------------------------------------------------------------

#ifdef _BEE_EXPORT
#	define BEE_API __declspec(dllexport)
#else
#	define BEE_API __declspec(dllimport)
#endif // !_BEE_EXPORT



// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

namespace Bee::Debug
{
    constexpr int     MaxPath = 0x0100; // 0x0100 (256)
    
    constexpr int     LoggerMessageMaxLenght = 0x0200; // 0x0200 (512)
    
    constexpr int     ExceptionLineNotCollected = -1;
    constexpr wchar_t ExceptionUnknownReason[]  = L"Unkonwn reason";
    constexpr wchar_t ExceptionUnknownFile[]    = L"Unkonwn file";
}



// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

#ifndef BEE_AS_WCHAR
#   define BEE_AS_WCHAR L""
#endif // !BEE_AS_WCHAR


#define BEE_COLLECT_DATA_ON_EXCEPTION() (Bee::Debug::ColletedOnException(BEE_AS_WCHAR __FILE__, __LINE__))

// Logger ---------------------------------------------------------------------
#if 1
#   define BEE_LOAD_LOGGER()  ::Bee::Debug::Logger::Get()
#   define BEE_CLOSE_LOGGER() ::Bee::Debug::Logger::Get().~Logger()

#	define BEE_LOGGER_SET_OUTPUT_PATH(szPath) ::Bee::Debug::Logger::Get().SetPath(szPath) 

#   define BEE_CREATE_IGNORED_MSG_LIST(...) { ::Bee::Debug::None, __VA_ARGS__ }

#   define BEE_LOGGER_SET_IGNORED_MSG_LIST(x)                         \
    ::Bee::Debug::Severity ___SeverityArrTmpPtr[] = x;                \
    ::Bee::Debug::Logger::Get().SetIgnoredTags(___SeverityArrTmpPtr); \

#	define BEE_LOG(...) ::Bee::Debug::Logger::Get().Log(__VA_ARGS__)

#	define BEE_LOAD_DEBUG()		\
           BEE_LOAD_LOGGER();	

#	define BEE_CLOSE_DEBUG()	\
           BEE_CLOSE_LOGGER();	

#else // ----------------------------------------------------------------------------
#   define BEE_LOAD_LOGGER()               
#   define BEE_CLOSE_LOGGER()              
#	define BEE_LOGGER_SET_OUTPUT_PATH(szPath) 
#   define BEE_CREATE_IGNORED_MSG_LIST(...)
#   define BEE_LOGGER_SET_IGNORED_MSG_LIST(x)
#	define BEE_LOG(...)
#   define BEE_LOG_WARNING(...) 
#   define BEE_LOG_ERROR(...) 
#	define BEE_LOAD_DEBUG()
#	define BEE_CLOSE_DEBUG()
#   define BEE_INCREMENT_MEMORY_LEAKS_TRACKER()
#   define BEE_DECREMENT_MEMORY_LEAKS_TRACKER()

#endif // Logger



// OpenGL ---------------------------------------------------------------------
#if 1
#   define BEE_GL(x)                                                                            \
    {                                                                                           \
        x;                                                                                      \
        auto _glError = glGetError();                                                           \
        if (_glError != GL_NO_ERROR)                                                            \
        {                                                                                       \
            BEE_LOG(::Bee::Debug::Error, L"OpenGL returned a following error %d", _glError);    \
        }                                                                                       \
    }                                                                                           \

#   define BEE_GLEW(x)                                                                                          \
    {                                                                                                           \
        auto _glError = x;                                                                                      \
        if (_glError != GLEW_OK)                                                                               \
        {                                                                                                       \
            BEE_LOG(::Bee::Debug::Error, L"GLEW returned a following error %S", glewGetErrorString(_glError));  \
        }                                                                                                       \
    }                                                                                                           \

#else
#   define BEE_GL(x) x
#   define BEE_GLEW(x) x
#endif // 1



// ----------------------------------------------------------------------------
// Project's headers
// ----------------------------------------------------------------------------

#include "Crash.hpp"
#include "Logger.hpp"
#include "Exception.hpp"
