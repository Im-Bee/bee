#pragma once

#ifndef BEE_API
#	ifdef _BEE_EXPORT
#		define BEE_API __declspec(dllexport)
#	else
#		define BEE_API __declspec(dllimport)
#	endif // !_BEE_EXPORT
#endif // !BEE_API

#ifdef _DEBUG
#	define BEE_LOAD_PROBLEMS()				\
            BEE_LOAD_LOGGER();	

#	define BEE_CLOSE_PROBLEMS()				\
            BEE_CLOSE_LOGGER();		

#	define B_LOG(...) Bee::Problems::Logger::Get().Log(__VA_ARGS__)
#	define BEE_LOGGER_SET_PATH(szPath) Bee::Problems::Logger::Get().SetPath(szPath) 
#   define BEE_LOGGER_SET_SUPPRESION(x) Bee::Problems::Logger::Get().SetSuppressed(x)
#else
#	define BEE_CLOSE_PROBLEMS()
#	define BEE_LOAD_PROBLEMS()
#	define B_LOG(...)
#	define BEE_LOGGER_SET_PATH(szPath) 
#   define BEE_LOGGER_SET_SUPPRESION(x)
#endif // _DEBUG

#define B_AS_WCHAR L""

#include "Logger.hpp"
#include "Exception.hpp"