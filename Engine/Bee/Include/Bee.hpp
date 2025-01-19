#pragma once

#include "../../BeeDebug/Include/BeeDebug.hpp"
#include "../../BeeUtil/Include/BeeUtil.hpp"
#include "../../BeeUtil/Include/Memory/Vector.hpp"

#ifndef BEE_API
#	ifdef _BEE_EXPORT
#		define BEE_API __declspec(dllexport)
#	else
#		define BEE_API __declspec(dllimport)
#	endif
#endif // !BEE_API

#ifndef BEE_WIN32
#   ifdef _WIN32
#      define BEE_WIN32

#   	ifndef WIN_LEAN_AND_MEAN
#   		define WIN_LEAN_AND_MEAN
#   	endif // !WIN_LEAN_AND_MEAN

#   	include <Windows.h>
#   endif // _WIN32
#endif // !BEE_WIN32

#include <cstdint>

#define BEE_MAX_PATH  ((unsigned char)(255))
#define BEE_BEE       (L"Bee")
#define BEE_WINDOW_UNKOWN_INDEX      (uint64_t(-1))
#define BEE_WINDOW_MAIN_WINDOW_INDEX (uint64_t( 0))
#define BEE_WINDOW_CLASS             (L"BeeWindow")

#define	B_HINSTANCE() GetModuleHandle(NULL)

#define B_CREATE_DIR(path)                                                           \
    if (!CreateDirectory(                                                            \
        path,                                                                        \
        NULL))                                                                       \
    {                                                                                \
        auto e = GetLastError();                                                     \
        if (e != ERROR_ALREADY_EXISTS)                                               \
            throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());  \
    }                                                                                \


#define B_GET_HANDLE(x)                                                      \
    x = this->GetHandle();                                                   \
    if (!x)                                                                  \
        throw ::Bee::Debug::NullptrCall(BEE_COLLECT_DATA_ON_EXCEPTION());    \

#pragma warning (push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning (disable : 4251)

namespace Bee 
{
    typedef Bee::Utils::Vec2<int32_t> Rectangle;
}

#include "Interfaces/IApplication.hpp"
#include "Interfaces/IWindow.hpp"
#include "Properties.hpp"
#include "Manager.hpp"

#include "Primitives/EmptyWindow.hpp"
#pragma warning (pop)
