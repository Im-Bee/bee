#pragma once

#ifdef _WIN32
#   include "Win32/EntryPoint.h"
#elif __linux__
#   include "Linux/EntryPoint.h"
#else
#   error "Couldn't detect the system"
#endif // !_WIN32
