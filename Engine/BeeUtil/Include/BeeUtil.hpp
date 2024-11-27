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

#ifndef B_SUCCESS_OPERATORS
#   define B_SUCCESS ((Bee::Utils::b_status)(0x01))
#   define B_OKAY    ((Bee::Utils::b_status)(0x10))
#   define B_BAD     ((Bee::Utils::b_status)(0x00))
#   define B_FAIL    ((Bee::Utils::b_status)(0xFF))

#   define B_RETURN_SUCCESS return B_SUCCESS
#   define B_RETURN_OKAY    { BEE_LOG(Bee::Problems::Warning, L"Function returned B_OKAY at line %d in file %S", __LINE__, __FILE__); return B_OKAY; }
#   define B_RETURN_FAIL    { BEE_LOG(Bee::Problems::Warning, L"Function returned B_FAIL at line %d in file %S", __LINE__, __FILE__); return B_FAIL; }
#   define B_RETURN_BAD     { BEE_LOG(Bee::Problems::Error,   L"Function returned B_BAD at line %d in file %S",  __LINE__, __FILE__); return B_FAIL; }

#   define B_IS_SUCCESS(x) ((x) == B_SUCCESS) 
#   define B_IS_OKAY(x)    ((x) >= B_SUCCESS) 
#   define B_IS_FAIL(x)    ((x) <= B_FAIL) 
#   define B_IS_BAD(x)     ((x) == B_BAD)

#   define B_WIN_SUCCEEDED(x)  SUCCEEDED(x)
#   define B_WIN_FAILED(x)     FAILED(x)

#   define B_THROW_IF_FAIL(x)                                                           \
    if (!B_IS_SUCCESS(x))                                                               \
        throw Bee::Problems::Exception(L"Operation didn't succed", BEE_COLLECT_DATA())    \

#   define B_WIN_THROW_IF_FAIL(x)                                                                               \
    if (!B_WIN_SUCCEEDED(x))                                                                                    \
    {                                                                                                           \
        BEE_LOG(Bee::Problems::Error, L"Operation didn't succed. Windows: GetLastError = %lu", GetLastError());   \
        throw Bee::Problems::Exception(L"Operation didn't succed. Windows", BEE_COLLECT_DATA());                  \
    }                                                                                                           \

#   define B_DXGI_THROW_IF_FAIL(x)                                      \
    {                                                                   \
        auto&& dxgiHresultReturn = x;                                   \
        if (B_WIN_FAILED(dxgiHresultReturn))                            \
        {                                                               \
            B_DXGI_REPORT_E(dxgiHresultReturn);                         \
            throw Bee::Problems::ProblemWithWINAPI(BEE_COLLECT_DATA());   \
        }                                                               \
    }                                                                   \

// Using this macro should log a GetLastError; might add some extra debuging in the future
#   define B_WIN_REPORT_FAILURE()      BEE_LOG(Problems::Error, L"Windows failed. GetLastError = %lu", GetLastError())
#   define B_DXGI_REPORT_E(x)          BEE_LOG(Problems::Error, L"DXGI failed. Error code = %x", x) 

#   define B_WIN_HANDLE_FAILURE_BEG(x) if (FAILED(x)) { B_WIN_REPORT_FAILURE()
#   define B_WIN_HANDLE_FAILURE_END    }

#   define B_DXGI_HANDLE_FAILURE_BEG(x) { auto&& dxgiHresultReturn = x; if (FAILED(dxgiHresultReturn)) { B_DXGI_REPORT_E(dxgiHresultReturn)
#   define B_DXGI_HANDLE_FAILURE_END    }}

#   define B_DXGI_IF_SUCCEEDED_BEG(x) { auto&& dxgiHresultReturn = x; if (SUCCEEDED(dxgiHresultReturn)) { 
#   define B_DXGI_IF_SUCCEEDED_END    } if (FAILED(dxgiHresultReturn)) { B_DXGI_REPORT_E(dxgiHresultReturn); } }
#endif // !B_SUCCESS_OPERATORS

namespace Bee::Utils
{
    typedef char b_status;

    template<typename T>
    struct Vec2
    {
        T x;
        T y;
    };

    template<typename T>
    struct Vec3
    {
        T x;
        T y;
        T z;
    };

    template<typename T>
    struct Vec4
    {
        T x;
        T y;
        T z;
        T w;
    };

    template <class T>
    struct RemoveRef { using Type = T; };

    template <class T>
    struct RemoveRef<T&> { using Type = T; };

    template <class T>
    struct RemoveRef<T&&> { using Type = T; };

    template <class T>
    constexpr typename RemoveRef<T>::Type&& Move(T&& arg) noexcept
    {
        return static_cast<RemoveRef<T>::Type&&>(arg);
    }
}

#include "Memory/Vector.hpp"
#include "Memory/List.hpp"
