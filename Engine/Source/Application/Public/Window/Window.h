#pragma once

#include "IWindow.h"
#include "WindowsManager.h"

#ifdef _WIN32
#   include "Window/Win32Window.h"
#elif __linux__
#   include "Window/LinuxWidnow.h"
#else
#   error "Couldn't detect the system"
#endif // !_WIN32

