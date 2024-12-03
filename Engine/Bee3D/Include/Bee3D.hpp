#pragma once

#ifndef _BEE_DX12
#   define _BEE_DX12
#endif // !_BEE_DX12
#ifndef _BEE_OPENGL
#   define _BEE_OPENGL
#endif // !_BEE_OPENGL

#include "../../BeeDebug/Include/BeeDebug.hpp"
#include "../../BeeUtil/Include/BeeUtil.hpp"
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

#include "../../BeeUtil/Include/SmartPointers/ComPtr.hpp"
#include "../../BeeUtil/Include/SmartPointers/SharedPtr.hpp"

#include <math.h> 

#ifdef _BEE_DX12
#   include <d3d12.h>
#   pragma comment(lib, "d3d12")
#   include <dxgi1_6.h>
#   pragma comment(lib, "dxgi")
#   include <DXGIDebug.h>
#   pragma comment(lib, "dxguid")
#   pragma comment(lib, "kernel32")

#   include "DX12/DxUtils.hpp"
#   include "DX12/IComponent.hpp"
#   include "DX12/RendererDX.hpp"
#endif // _BEE_DX12

#ifdef _BEE_OPENGL
#   include "gl/glew.h"
#   include <gl/GL.h>
#   include <gl/GLU.h>
#   include "glfw3.h"

#   include "OpenGL/Primitives/OpenGLWindow.hpp"
#   include "OpenGL/RendererGL.hpp"
#endif // _BEE_OPENGL
