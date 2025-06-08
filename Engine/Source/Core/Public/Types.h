#pragma once

#ifdef _WIN32
#   include "WindowsTypes.h"
#elif __linux__
#   include "LinuxTypes.h"
#else
#   error "Couldn't detect the system"
#endif // !_WIN32


