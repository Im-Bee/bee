#pragma once

#ifndef _BEE_DX12
#   define _BEE_DX12
#endif // !_BEE_DX12

#include "../../BeeDebug/Include/BeeDebug.hpp"
#include "../../BeeUtil/Include/BeeUtil.hpp"
#include "../../BeeUtil/Include/SmartPointers/ComPtr.hpp"
#include "../../Bee/Include/Bee.hpp"

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
#endif // !_WIN32

#ifdef _BEE_DX12
#   include <d3d12.h>
#endif // _BEE_DX12

