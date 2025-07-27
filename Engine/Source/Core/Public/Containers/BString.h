#pragma once


namespace Duckers
{

class String
{
public:

    String() = default;
    ~String() = default;

    String(String&&)
    {
        throw; // TODO: ...
    }

    String(const String&) 
    {
        throw; // TODO: ...
    }

public:

    const char* Raw() const
    { return ""; }

};

} // !Duckers
