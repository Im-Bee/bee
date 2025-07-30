#pragma once

#include "CoreMinimal.h"

namespace Duckers
{

class DUCKERS_API File
{
public:

    File() 
        : m_wszFilePath(nullptr)
    { }

    explicit File(const wchar_t* wszPathToFile) 
        : m_wszFilePath(wszPathToFile)
    { } 

    ~File() = default;

    File(File&&) = default;
    File(const File&) = default;

public:

private:

    const wchar* m_wszFilePath;

};


} // !Duckers
