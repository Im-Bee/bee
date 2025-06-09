#pragma once


#ifdef _WIN32
#   include "WindowsMinimalInclude.h"
#elif __linux__
#   include "LinuxMinimalInclude.h"
#else
#   error "Couldn't detect the system"
#endif // !_WIN32

