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
#   define BEE_ALREADY_DID  (::Bee::Utils::b_status(0x7F))
#   define BEE_COULDNT_DO   (::Bee::Utils::b_status(0xFF))
#   define BEE_CORRUPTION   (::Bee::Utils::b_status(0x00))



#   define BEE_IS_SUCCESS(x)     ((x) == BEE_SUCCESS) 
#   define BEE_IS_ALREADY_DID(x) ((x) >= BEE_SUCCESS) 
#   define BEE_IS_COULDNT_DO(x)  ((x) <= BEE_CORRUPTION) 
#   define BEE_IS_CORRUPTED(x)   ((x) == BEE_CORRUPTION)



#   define BEE_WIN_IS_SUCCESS(x)  SUCCEEDED(x)
#   define BEE_WIN_IS_FAIL(x)     FAILED(x)



#   define BEE_THROW_IF_FAILED(x)                                                                       \
    if (!BEE_IS_SUCCESS(x))                                                                             \
    {                                                                                                   \
        throw ::Bee::Debug::Exception(L"Operation wasn't successful", BEE_COLLECT_DATA_ON_EXCEPTION()); \
    }                                                                                                   \



#   define BEE_WIN_THROW_IF_FAIL(x)                                                                               \
    if (!BEE_WIN_IS_SUCCESS(x))                                                                                   \
    {                                                                                                             \
        BEE_LOG(::Bee::Debug::Error, L"Operation didn't succed. Windows: GetLastError = %lu", GetLastError());    \
        throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());                                   \
    }                                                                                                             \



#   define BEE_DXGI_THROW_IF_FAIL(x)                                                    \
    {                                                                                   \
        auto _dxgiHresultReturn = x;                                                    \
        if (BEE_WIN_IS_FAIL(_dxgiHresultReturn))                                        \
        {                                                                               \
            B_DXGI_REPORT_E(_dxgiHresultReturn);                                        \
            throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());     \
        }                                                                               \
    }                                                                                   \



    // Using this macro should log a GetLastError; might add some extra debuging in the future
#   define B_REPORT_FAILURE()          BEE_LOG(::Bee::Debug::Error, L"Bee failed at line %d in file '%S'", __LINE__, __FILE__);
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

        Vec3& operator+=(const T& other)
        {
            this->x += other;
            this->y += other;
            this->z += other;

            return *this;
        }
        
        Vec3  operator-(const Vec3& other) const
        {
            return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
        }

        Vec3& operator-=(const Vec3& other)
        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;

            return *this;
        }

        Vec3& operator-=(const T& other)
        {
            this->x -= other;
            this->y -= other;
            this->z -= other;

            return *this;
        }

        Vec3  operator*(const Vec3& other) const
        {
            return Vec3(this->x * other.x, this->y * other.y, this->z * other.z);
        }

        Vec3& operator*=(const Vec3& other)
        {
            this->x *= other.x;
            this->y *= other.y;
            this->z *= other.z;

            return *this;
        }

        Vec3  operator*(const T& num) const
        {
            return Vec3(this->x * num, this->y * num, this->z * num);
        }

        Vec3  operator/(const Vec3& other) const
        {
            return Vec3(this->x / other.x, this->y / other.y, this->z / other.z);
        }

        Vec3& operator/=(const Vec3& other)
        {
            this->x /= other.x;
            this->y /= other.y;
            this->z /= other.z;

            return *this;
        }

        Vec3  operator/(const T& num) const
        {
            return Vec3(this->x / num, this->y / num, this->z / num);
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

        Vec3  Normalize()
        {
            auto lenght(sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z)));
            return Vec3(this->x / lenght, this->y / lenght, this->z / lenght);
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


        Vec4  operator+(const Vec4& other) const
        {
            return Vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
        }

        Vec4& operator+=(const Vec4& other)
        {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            this->w += other.w;

            return *this;
        }

        Vec4  operator-(const Vec4& other) const
        {
            return Vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w * other.w);
        }

        Vec4& operator-=(const Vec4& other)
        {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
            this->w -= other.w;

            return *this;
        }

        Vec4  operator*(const Vec4& other) const
        {
            return Vec4(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
        }

        Vec4& operator*=(const Vec4& other)
        {
            this->x *= other.x;
            this->y *= other.y;
            this->z *= other.z;
            this->w *= other.w;

            return *this;
        }

        Vec4  operator*(const T& num) const
        {
            return Vec4(this->x * num, this->y * num, this->z * num, this->w * num);
        }

        Vec4  operator/(const Vec4& other) const
        {
            return Vec4(this->x / other.x, this->y / other.y, this->z / other.z, this-w / other.w);
        }

        Vec4& operator/=(const Vec4& other)
        {
            this->x /= other.x;
            this->y /= other.y;
            this->z /= other.z;
            this->w /= other.w;

            return *this;
        }

        Vec4  operator/(const T& num) const
        {
            return Vec4(this->x / num, this->y / num, this->z / num, this->w / num);
        }

        float DotProduct(const Vec4& other) const
        {
            return (this->x * other.x) + (this->y * other.y) + (this->z * other.z) + (this->w * other.w);
        }

        bool operator==(const Vec4& other) const
        {
            if (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w)
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
    struct Mat3x3
    {
        T m11; T m12; T m13;
        T m21; T m22; T m23;
        T m31; T m32; T m33;
    };

    template<class T>
    void MultiplyMat3x3Vec3(const Mat3x3<T>& m, const Vec3<T>& v, Vec3<T>& vr)
    {
        vr.x = (m.m11 * v.x) + (m.m12 * v.y) + (m.m13 * v.z);
        vr.y = (m.m21 * v.x) + (m.m22 * v.y) + (m.m23 * v.z);
        vr.z = (m.m31 * v.x) + (m.m32 * v.y) + (m.m33 * v.z);
    }

    template<class T>
    void MultiplyMat3x3(const Mat3x3<T>& m1, const Mat3x3<T>& m2, Mat3x3<T>& mr)
    {
        mr.m11 = (m1.m11 * m2.m11) + (m1.m12 * m2.m21) + (m1.m13 * m2.m31);
        mr.m12 = (m1.m11 * m2.m12) + (m1.m12 * m2.m22) + (m1.m13 * m2.m32);
        mr.m13 = (m1.m11 * m2.m13) + (m1.m12 * m2.m23) + (m1.m13 * m2.m33);
                                                                          
        mr.m21 = (m1.m21 * m2.m11) + (m1.m22 * m2.m21) + (m1.m23 * m2.m31);
        mr.m22 = (m1.m21 * m2.m12) + (m1.m22 * m2.m22) + (m1.m23 * m2.m32);
        mr.m23 = (m1.m21 * m2.m13) + (m1.m22 * m2.m23) + (m1.m23 * m2.m33);
                                                                          
        mr.m31 = (m1.m31 * m2.m11) + (m1.m32 * m2.m21) + (m1.m33 * m2.m31);
        mr.m32 = (m1.m31 * m2.m12) + (m1.m32 * m2.m22) + (m1.m33 * m2.m32);
        mr.m33 = (m1.m31 * m2.m13) + (m1.m32 * m2.m23) + (m1.m33 * m2.m33);
    }

    template<class T>
    Mat3x3<T> CreateYRotationMat3x3(T rotationInRad)
    {
        return Mat3x3<T>( cos(rotationInRad), 0.f, sin(rotationInRad),
                                         0.f, 1.f,                0.f,
                         -sin(rotationInRad), 0.f, cos(rotationInRad));
    }

    template<typename T>
    struct Mat4x4
    {
        T m11; T m12; T m13; T m14;
        T m21; T m22; T m23; T m24;
        T m31; T m32; T m33; T m34;
        T m41; T m42; T m43; T m44;
    };

    template<class T>
    void MultiplyMat4x4Vec3(const Mat4x4<T>& m, const Vec3<T>& v, Vec3<T>& vr)
    {
        vr.x = (m.m11 * v.x) + (m.m12 * v.y) + (m.m13 * v.z) + m.m14;
        vr.y = (m.m21 * v.x) + (m.m22 * v.y) + (m.m23 * v.z) + m.m24;
        vr.z = (m.m31 * v.x) + (m.m32 * v.y) + (m.m33 * v.z) + m.m34;
    }

    template<class T>
    void MultiplyMat4x4(const Mat4x4<T>& m1, const Mat4x4<T>& m2, Mat4x4<T>& mr)
    {
        mr.m11 = (m1.m11 * m2.m11) + (m1.m12 * m2.m21) + (m1.m13 * m2.m31) + (m1.m14 * m2.m41);
        mr.m12 = (m1.m11 * m2.m12) + (m1.m12 * m2.m22) + (m1.m13 * m2.m32) + (m1.m14 * m2.m42);
        mr.m13 = (m1.m11 * m2.m13) + (m1.m12 * m2.m23) + (m1.m13 * m2.m33) + (m1.m14 * m2.m43);
        mr.m14 = (m1.m11 * m2.m14) + (m1.m12 * m2.m24) + (m1.m13 * m2.m34) + (m1.m14 * m2.m44);

        mr.m21 = (m1.m21 * m2.m11) + (m1.m22 * m2.m21) + (m1.m23 * m2.m31) + (m1.m24 * m2.m41);
        mr.m22 = (m1.m21 * m2.m12) + (m1.m22 * m2.m22) + (m1.m23 * m2.m32) + (m1.m24 * m2.m42);
        mr.m23 = (m1.m21 * m2.m13) + (m1.m22 * m2.m23) + (m1.m23 * m2.m33) + (m1.m24 * m2.m43);
        mr.m24 = (m1.m21 * m2.m14) + (m1.m22 * m2.m24) + (m1.m23 * m2.m34) + (m1.m24 * m2.m44);

        mr.m31 = (m1.m31 * m2.m11) + (m1.m32 * m2.m21) + (m1.m33 * m2.m31) + (m1.m34 * m2.m41);
        mr.m32 = (m1.m31 * m2.m12) + (m1.m32 * m2.m22) + (m1.m33 * m2.m32) + (m1.m34 * m2.m42);
        mr.m33 = (m1.m31 * m2.m13) + (m1.m32 * m2.m23) + (m1.m33 * m2.m33) + (m1.m34 * m2.m43);
        mr.m34 = (m1.m31 * m2.m14) + (m1.m32 * m2.m24) + (m1.m33 * m2.m34) + (m1.m34 * m2.m44);

        mr.m41 = (m1.m41 * m2.m11) + (m1.m42 * m2.m21) + (m1.m43 * m2.m31) + (m1.m44 * m2.m41);
        mr.m42 = (m1.m41 * m2.m12) + (m1.m42 * m2.m22) + (m1.m43 * m2.m32) + (m1.m44 * m2.m42);
        mr.m43 = (m1.m41 * m2.m13) + (m1.m42 * m2.m23) + (m1.m43 * m2.m33) + (m1.m44 * m2.m43);
        mr.m44 = (m1.m41 * m2.m14) + (m1.m42 * m2.m24) + (m1.m43 * m2.m34) + (m1.m44 * m2.m44);
    }

    template<class T>
    void TransposeMat4x4(Mat4x4<T>& t)
    {
        T tmp;

        tmp = t.m12;
        t.m12 = t.m21;
        t.m21 = tmp;


        tmp = t.m13;
        t.m13 = t.m31;
        t.m31 = tmp;


        tmp = t.m14;
        t.m14 = t.m41;
        t.m41 = tmp;


        tmp = t.m23;
        t.m23 = t.m32;
        t.m32 = tmp;


        tmp = t.m24;
        t.m24 = t.m42;
        t.m42 = tmp;


        tmp = t.m34;
        t.m34 = t.m43;
        t.m43 = tmp;
    }

    typedef ::Bee::Utils::Vec2<float>   Rectangle;
    typedef ::Bee::Utils::Vec2<float>   Vec2f;
    typedef ::Bee::Utils::Vec3<float>   Vec3f;
    typedef ::Bee::Utils::Vec4<float>   Vec4f;
    typedef ::Bee::Utils::Mat3x3<float> Mat3x3f;
    typedef ::Bee::Utils::Mat4x4<float> Mat4x4f;

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

#   define BEE_INVALID_VECTOR_3F   ::Bee::Utils::Vec3f(BEE_INFINITY, BEE_INFINITY, BEE_INFINITY)
}



// ----------------------------------------------------------------------------
// Project's headers
// ----------------------------------------------------------------------------

#include "Memory/Vector.hpp"
#include "Memory/List.hpp"
