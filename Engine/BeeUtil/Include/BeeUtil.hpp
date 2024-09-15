#pragma once

#include "../../BeeDebug/Include/BeeDebug.hpp"

#ifndef BEE_API
#	ifdef _BEE_EXPORT
#		define BEE_API __declspec(dllexport)
#	else
#		define BEE_API __declspec(dllimport)
#	endif

#endif // !BEE_API

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN

#	include <Windows.h>
#endif // _WIN32

namespace Bee::Utils
{
    typedef char b_success;

#ifndef B_SUCCESS_OPERATORS
#define B_SUCCESS ((char)(0x01))
#define B_FAIL    ((char)(0x00))

#define B_IS_FAIL(x) ((x) <= B_FAIL) 
#define B_IS_SUCCESS(x) (!B_IS_SUCCESS(x)) 
#endif // !B_SUCCESS_OPERATORS


    template<typename T>
    struct Vec2
    {
        T x = {};
        T y = {};
    };

    template<typename T>
    struct Vec3
    {
        T x = {};
        T y = {};
        T z = {};
    };

    template<typename T>
    struct Vec4
    {
        T x = {};
        T y = {};
        T z = {};
        T w = {};
    };
}

#include "Vector.hpp"
#include "List.hpp"
