#pragma once

#include "../../BeeDebug/Include/BeeDebug.hpp"
#include "../../BeeUtil/Include/BeeUtil.hpp"

// ----------------------------------------------------------------------------
// System headers
// ----------------------------------------------------------------------------

#ifdef _WIN32
#	ifndef WIN_LEAN_AND_MEAN
#		define WIN_LEAN_AND_MEAN
#	endif // !WIN_LEAN_AND_MEAN
#	include <Windows.h>
#endif // _WIN32


#include <cstdint>



// ----------------------------------------------------------------------------
// API import/export macros
// ----------------------------------------------------------------------------

#ifdef _BEE_EXPORT
#	define BEE_API __declspec(dllexport)
#else
#	define BEE_API __declspec(dllimport)
#endif // !_BEE_EXPORT



// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------



#define BEE_MAX_PATH                 ((unsigned char)(255))
#define BEE_BEE                      (L"Bee")
#define BEE_WINDOW_UNKOWN_INDEX      (uint64_t(-1))
#define BEE_WINDOW_MAIN_WINDOW_INDEX (uint64_t( 0))
#define BEE_WINDOW_CLASS             (L"BeeWindow")

#define	B_HINSTANCE() GetModuleHandle(NULL)

#define B_CREATE_DIR(path)                                                           \
    if (!CreateDirectory(path,                                                       \
                         NULL))                                                      \
    {                                                                                \
        auto e = GetLastError();                                                     \
        if (e != ERROR_ALREADY_EXISTS)                                               \
            throw ::Bee::Debug::ProblemWithWINAPI(BEE_COLLECT_DATA_ON_EXCEPTION());  \
    }                                                                                \



#define B_GET_HANDLE(x)                                                      \
    x = this->GetHandle();                                                   \
    if (!x)                                                                  \
    {                                                                        \
        throw ::Bee::Debug::NullptrCall(BEE_COLLECT_DATA_ON_EXCEPTION());    \
    }                                                                        \



namespace Bee
{
}



#pragma warning (push)
// Warning	C4251	Needs to have dll to be used by clients of class
#pragma warning (disable : 4251)
#include "Interfaces/IApplication.hpp"
#include "Interfaces/IWindow.hpp"
#include "Properties.hpp"
#include "Manager.hpp"

#include "Primitives/EmptyWindow.hpp"
#pragma warning (pop)
