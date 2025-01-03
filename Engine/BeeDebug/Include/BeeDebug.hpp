#pragma once

// ----------------------------------------------------------------------------
// System headers
// ----------------------------------------------------------------------------

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN
#	include <Windows.h>
#endif // _WIN32

// ----------------------------------------------------------------------------
// API
// ----------------------------------------------------------------------------

#ifndef BEE_API
#	ifdef _BEE_EXPORT
#		define BEE_API __declspec(dllexport)
#	else
#		define BEE_API __declspec(dllimport)
#	endif // !_BEE_EXPORT
#endif // !BEE_API

// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

namespace Bee::Problems
{
    constexpr int MaxPath                = 0x100;
    constexpr int LoggerMessageMaxLenght = 0x1000;
}

// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

#ifndef BEE_AS_WCHAR
#   define BEE_AS_WCHAR L""
#endif // !BEE_AS_WCHAR

#define BEE_COLLECT_DATA() (Bee::Problems::CollectedData(BEE_AS_WCHAR __FILE__, __LINE__))

#if 1
// Logger ---------------------------------------------------------------------
#   define BEE_LOAD_LOGGER()                Bee::Problems::Logger::Get()
#   define BEE_CLOSE_LOGGER()               Bee::Problems::Logger::Get().~Logger()

#	define BEE_LOGGER_SET_OUT_PATH(szPath)  Bee::Problems::Logger::Get().SetPath(szPath) 

#   define BEE_CREATE_IGNORE_LIST(...)      { None, __VA_ARGS__ }
#   define BEE_LOGGER_SET_IGNORE_LIST(x)            \
    {                                               \
        static Severity SeverityTmpPtr[] = x;       \
        Logger::Get().SetIgnore(SeverityTmpPtr);    \
    }                                               \

#	define BEE_LOG(...)                     { Bee::Problems::Logger::Get().Log(__VA_ARGS__); }

// MemoryLeaks ----------------------------------------------------------------

#   define BEE_INCREMENT_MEMORY_LEAKS_TRACKER() Bee::Problems::IMemoryLeaksTracker::Increment()
#   define BEE_DECREMENT_MEMORY_LEAKS_TRACKER() Bee::Problems::IMemoryLeaksTracker::Decrement()

// ----------------------------------------------------------------------------

#	define BEE_LOAD_DEBUG()		\
           BEE_LOAD_LOGGER();	

#	define BEE_CLOSE_DEBUG()	\
           BEE_CLOSE_LOGGER();		
#else
#   define BEE_LOAD_LOGGER()               
#   define BEE_CLOSE_LOGGER()              
#	define BEE_LOGGER_SET_OUT_PATH(szPath) 
#   define BEE_CREATE_IGNORE_LIST(...)
#   define BEE_LOGGER_SET_IGNORE_LIST(x)
#	define BEE_LOG(...)
#	define BEE_LOAD_DEBUG()
#	define BEE_CLOSE_DEBUG()
#   define BEE_INCREMENT_MEMORY_LEAKS_TRACKER()
#   define BEE_DECREMENT_MEMORY_LEAKS_TRACKER()
#endif // _DEBUG

// ----------------------------------------------------------------------------
// Project headers
// ----------------------------------------------------------------------------

#include "Crash.hpp"
#include "Logger.hpp"
#include "Exception.hpp"
#include "MemoryLeaks.hpp"