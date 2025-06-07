#pragma once

#ifdef _WIN32
#   ifdef _DUCKERS_API_EXPORT
#       define DUCKERS_API __declspec (dllexport)
#   else
#       define DUCKERS_API __declspec (dllimport)
#   endif // !_DUCKERS_API_EXPORT
#elif __linux__
#   ifdef _DUCKERS_API_EXPORT
#       define DUCKERS_API __atribute__ ((visibilty ("default")))
#   else
#       define DUCKERS_API
#   endif // !_DUCKERS_API_EXPORT
#else
#   error "Couldn't detect the system"
#endif // !_WIN32

