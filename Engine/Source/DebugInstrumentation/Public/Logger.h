#pragma once 

#include "CoreMinimal.h"

namespace Duckers
{


class DUCKERS_API Logger
{
    Logger() = default;
    
public:

    ~Logger() = default;

    static Logger& Get()
    {
        static Logger* pInstance = nullptr;
        if (!pInstance) {
            pInstance = new Logger();
        }

        return *pInstance;
    }

    Logger(const Logger&) = default;
    Logger(Logger&&) = delete;

private:



};


} // !Duckers
