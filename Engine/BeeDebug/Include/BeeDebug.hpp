#pragma once

#ifndef BEE_API
#	ifdef _BEE_EXPORT
#		define BEE_API __declspec(dllexport)
#	else
#		define BEE_API __declspec(dllimport)
#	endif // !_BEE_EXPORT
#endif // !BEE_API

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN
#	include <Windows.h>
#endif // _WIN32

#ifndef B_AS_WCHAR
#   define B_AS_WCHAR L""
#endif // !B_AS_WCHAR

namespace Bee::Problems
{
    constexpr unsigned char MaxPath                = 0xFF;
    constexpr unsigned char LoggerMessageMaxLenght = 0xFF;
}


#ifdef _DEBUG
#   define BEE_LOAD_LOGGER()                Bee::Problems::Logger::Get()
#   define BEE_CLOSE_LOGGER()               Bee::Problems::Logger::Get().~Logger()
#   define BEE_CREATE_SUPPRESSION_LIST(...) Bee::Problems::SuppressionList({__VA_ARGS__})

#	define BEE_LOAD_PROBLEMS()		\
           BEE_LOAD_LOGGER();	

#	define BEE_CLOSE_PROBLEMS()	    \
           BEE_CLOSE_LOGGER();		

#	define B_LOG(...)                       { Bee::Problems::Logger::Get().Log(__VA_ARGS__); }
#	define BEE_LOGGER_SET_PATH(szPath)      Bee::Problems::Logger::Get().SetPath(szPath) 
#   define BEE_LOGGER_SET_SUPPRESION(x)     Bee::Problems::Logger::Get().SetSuppressed(x)
#   define BEE_CREATE_SUPPRESSION_LIST(...) Bee::Problems::SuppressionList({__VA_ARGS__})
#else

#   define BEE_LOAD_LOGGER()               
#   define BEE_CLOSE_LOGGER()              
#   define BEE_CREATE_SUPPRESSION_LIST(...)
#	define BEE_CLOSE_PROBLEMS()
#	define BEE_LOAD_PROBLEMS()
#	define B_LOG(...)
#	define BEE_LOGGER_SET_PATH(szPath) 
#   define BEE_LOGGER_SET_SUPPRESION(x)
#   define BEE_CREATE_SUPPRESSION_LIST(...) 
#endif // _DEBUG


#include "Crash.hpp"
#include "Logger.hpp"
#include "Exception.hpp"