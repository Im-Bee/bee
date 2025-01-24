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

#include <cmath> 

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
#   define BEE_HUGE_NUM         (static_cast<float>(1e+300))
#   define BEE_INFINITY         BEE_HUGE_NUM

#   define BEE_EPSILON          1.192092896e-07f

#   define BEE_PI               3.14159f
#   define BEE_TWO_PI           6.28319f
    
#   define BEE_DEG_TO_RADIAN    0.01745329f

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

        Vec3  operator+(const Vec3& other) const
        {
            return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
        }

        Vec3& operator+=(const Vec3& other)
        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;

            return *this;
        }

        Vec3& operator-=(const Vec3& other)
        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;

            return *this;
        }

        Vec3  operator-(const Vec3& other) const
        {
            return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
        }

        Vec3  operator*(const Vec3& other) const
        {
            return Vec3(this->x * other.x, this->y * other.y, this->z * other.z);
        }

        Vec3  operator*(const float& num) const
        {
            return Vec3(this->x * num, this->y * num, this->z * num);
        }

        float DotProduct(const Vec3& other) const
        {
            return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
        }

        Vec3  CrossProduct(const Vec3& other) const
        {
            return Vec3((this->y * other.z) - (this->z * other.y),
                        (this->z * other.x) - (this->x * other.z),
                        (this->x * other.y) - (this->y * other.x));
        }

        bool operator==(const Vec3& other) const
        {
            if (this->x == other.x && this->y == other.y && this->z == other.z)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
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

    typedef ::Bee::Utils::Vec3<float> Vec3f;

    template<typename T>
    struct Mat3x3
    {
        T m11;
        T m12;
        T m13;

        T m21;
        T m22;
        T m23;

        T m31;
        T m32;
        T m33;
    };

    typedef ::Bee::Utils::Mat3x3<float> Mat3x3f;

    template<class T>
    void MatMulVec(const Mat3x3<T>& m, const Vec3<T>& v, Vec3<T>& vr)
    {
        vr.x = (m.m11 * v.x) + (m.m12 * v.y) + (m.m13 * v.z);
        vr.y = (m.m21 * v.x) + (m.m22 * v.y) + (m.m23 * v.z);
        vr.z = (m.m31 * v.x) + (m.m32 * v.y) + (m.m33 * v.z);
    }

    template<class T>
    Mat3x3<T> CreateRotationYMat(T rotationInRad)
    {
        return Mat3x3<T>( cos(rotationInRad), 0.f, sin(rotationInRad),
                                         0.f, 1.f,                0.f,
                         -sin(rotationInRad), 0.f, cos(rotationInRad));
    }

#   define BEE_INVALID_VECTOR_3F   ::Bee::Utils::Vec3f(BEE_INFINITY, BEE_INFINITY, BEE_INFINITY)

    struct Triangle3f
    {
        using Vec3f = ::Bee::Utils::Vec3f;

        Vec3f p0;
        Vec3f p1;
        Vec3f p2;
    };

    template<class TA, class TB>
    struct Pair
    {
        TA a;
        TB b;
    };
}



// ----------------------------------------------------------------------------
// Project's headers
// ----------------------------------------------------------------------------

#include "Memory/Vector.hpp"
#include "Memory/List.hpp"
