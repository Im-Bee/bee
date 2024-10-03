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

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN

#	include <Windows.h>
#endif // _WIN32

#define	B_HINSTANCE() GetModuleHandle(NULL)
#define B_MAX_PATH ((unsigned char)(255))
#define B_BEE (L"Bee")

#define B_CREATE_DIR(path)                                          \
if (!CreateDirectory(                                               \
    path,                                                           \
    NULL))                                                          \
{                                                                   \
    auto e = GetLastError();                                        \
    if (e != ERROR_ALREADY_EXISTS)                                  \
        throw Problems::ProblemWithWINAPI(BEE_COLLECT_DATA());      \
}

#pragma warning (push)
#pragma warning (disable : 4251)

#include "Interfaces/IApplication.hpp"
#include "Interfaces/IWindow.hpp"
#include "Properties.hpp"
#include "Manager.hpp"

#pragma warning (pop)
