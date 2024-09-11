#pragma once

#ifndef BEE_API
#	ifdef _BEE_EXPORT
#		define BEE_API __declspec(dllexport)
#	else
#		define BEE_API __declspec(dllimport)
#	endif // !_BEE_EXPORT
#endif // !BEE_API

#define B_COMMA ,

#ifdef _DEBUG
#	include "Logger.hpp"

#	define BEE_LOAD_PROBLEMS()				\
            BEE_LOAD_LOGGER();					

#	define B_LOG(...) Bee::Problems::Logger::Get().Log(__VA_ARGS__)
#	define BEE_LOGGER_SET_PATH(szPath) Bee::Problems::Logger::Get().SetPath(szPath) 

#else
#	define BEE_LOAD_PROBLEMS()
#	define B_LOG(...)
#	define B_LOGGER_SET_PATH(szPath) 
#endif // _DEBUG

#include "Exception.hpp"
