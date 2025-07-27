#pragma once

#include "Logger.h"


#ifdef _DUCKERS_LOG_DATA

#   ifndef D_LOG
#      define D_LOG(...) ::Duckers::Debug::Logger::Get().LogSingleThreaded(__VA_ARGS__)
#   endif // !LOG

#   ifndef D_INFO
#      define D_INFO ::Duckers::Debug::ESeverity::Info
#   endif // !D_INFO
          
#   ifndef D_WARNING
#      define D_WARNING ::Duckers::Debug::ESeverity::Warning
#   endif // !D_WARNING
          
#   ifndef D_ERROR
#      define D_ERROR ::Duckers::Debug::ESeverity::Error
#   endif // !D_ERROR

#else

#   ifndef D_LOG
#      define D_LOG(...) 
#   endif // !LOG

#   ifndef D_INFO
#      define D_INFO 
#   endif // !D_INFO
          
#   ifndef D_WARNING
#      define D_WARNING 
#   endif // !D_WARNING
          
#   ifndef D_ERROR
#      define D_ERROR 
#   endif // !D_ERROR
       
#endif // !_DUCKERS_LOG_DATA
