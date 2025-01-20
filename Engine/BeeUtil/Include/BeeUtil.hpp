#pragma once

#include "../../BeeDebug/Include/BeeDebug.hpp"



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
// API import/export macros
// ----------------------------------------------------------------------------

#ifdef _BEE_EXPORT
#	define BEE_API __declspec(dllexport)
#else
#	define BEE_API __declspec(dllimport)
#endif // !_BEE_EXPORT



// ----------------------------------------------------------------------------
// Success and fail operators
// ----------------------------------------------------------------------------

#ifndef BEE_SUCCESS_OPERATORS
#   define BEE_SUCCESS_OPERATORS

#   define BEE_SUCCESS      (::Bee::Utils::b_status(0x01))
#   define BEE_OKAY         (::Bee::Utils::b_status(0x7F))
#   define BEE_FAIL         (::Bee::Utils::b_status(0xFF))
#   define BEE_CORRUPTION   (::Bee::Utils::b_status(0x00))



#   define BEE_RETURN_SUCCESS return BEE_SUCCESS



#   define BEE_RETURN_OKAY                                                                                              \
    return BEE_OKAY;                                                                                                    \



#   define BEE_RETURN_FAIL                                                                                              \
    BEE_LOG(::Bee::Debug::Warning, L"Function returned BEE_FAIL at line %d in file %S", __LINE__, __FILE__);            \
    return BEE_FAIL;                                                                                                    \



#   define BEE_RETURN_BAD                                                                                               \
    BEE_LOG(::Bee::Debug::Error, L"Function returned BEE_CORRUPTION at line %d in file %S", __LINE__, __FILE__);        \
    return BEE_CORRUPTION;                                                                                              \



#   define BEE_SUCCEEDED(x)     ((x) == BEE_SUCCESS) 
#   define BEE_IS_OKAY(x)       ((x) >= BEE_SUCCESS) 
#   define BEE_FAILED(x)        ((x) <= BEE_CORRUPTION) 
#   define BEE_CORRUPTED(x)     ((x) == BEE_CORRUPTION)



#   define B_WIN_SUCCEEDED(x)  SUCCEEDED(x)
#   define B_WIN_FAILED(x)     FAILED(x)



#   define BEE_THROW_IF_FAILED(x)                                                                       \
    if (!BEE_SUCCEEDED(x))                                                                              \
    {                                                                                                   \
        throw ::Bee::Debug::Exception(L"Operation wasn't succed", BEE_COLLECT_DATA_ON_EXCEPTION());     \
    }                                                                                                   \



#   define B_WIN_THROW_IF_FAIL(x)                                                                                 \
    if (!B_WIN_SUCCEEDED(x))                                                                                      \
    {                                                                                                             \
        BEE_LOG(::Bee::Debug::Error, L"Operation didn't succed. Windows: GetLastError = %lu", GetLastError());    \
        throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());                                   \
    }                                                                                                             \



#   define B_DXGI_THROW_IF_FAIL(x)                                                      \
    {                                                                                   \
        auto _dxgiHresultReturn = x;                                                    \
        if (B_WIN_FAILED(_dxgiHresultReturn))                                           \
        {                                                                               \
            B_DXGI_REPORT_E(_dxgiHresultReturn);                                        \
            throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());     \
        }                                                                               \
    }                                                                                   \



    // Using this macro should log a GetLastError; might add some extra debuging in the future
#   define B_WIN_REPORT_FAILURE()      BEE_LOG(::Bee::Debug::Error, L"Windows failed. GetLastError = %lu at line %d in file '%S'", GetLastError(), __LINE__, __FILE__);
#   define B_DXGI_REPORT_E(x)          BEE_LOG(::Bee::Debug::Error, L"DXGI failed. Error code = %x at line %d in file '%S'", x, __LINE__, __FILE__);



#   define B_WIN_HANDLE_FAILURE_BEG(x)      \
    if (FAILED(x))                          \
    {                                       \
        B_WIN_REPORT_FAILURE()              \



#   define B_WIN_HANDLE_FAILURE_END    }



#   define B_DXGI_HANDLE_FAILURE_BEG(x)                         \
    {                                                           \
        auto _dxgiHresultReturn = x;                            \
        if (FAILED(_dxgiHresultReturn))                         \
        {                                                       \
            B_DXGI_REPORT_E(_dxgiHresultReturn);                \




#   define B_DXGI_HANDLE_FAILURE_END    }}



#   define B_DXGI_IF_SUCCEEDED_BEG(x)           \
    {                                           \
        auto _dxgiHresultReturn = x;            \
        if (SUCCEEDED(_dxgiHresultReturn))      \
        {                                       \



#   define B_DXGI_IF_SUCCEEDED_END                  \
        }                                           \
        else                                        \
        {                                           \
            B_DXGI_REPORT_E(_dxgiHresultReturn);    \
        }                                           \
    }                                               \



#endif // !BEE_SUCCESS_OPERATORS



namespace Bee::Utils
{
#   define BEE_HUGE_NUM        (static_cast<float>(1e+300))
#   define BEE_INFINITY         BEE_HUGE_NUM

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

    typedef ::Bee::Utils::Vec2<float> Rectangle;
}



// ----------------------------------------------------------------------------
// Project's headers
// ----------------------------------------------------------------------------

#include "Memory/Vector.hpp"
#include "Memory/List.hpp"
