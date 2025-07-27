#pragma once


namespace Duckers
{

class String
{
public:

    String() 
        : m_pszString(nullptr)
        , m_bOwnMemory(true)
    { }

    ~String() 
    {
        if (m_bOwnMemory && m_pszString) {
            DeAllocateString();
        }
    }

    explicit String(const char pszString[]) 
        : m_pszString(const_cast<char*>(pszString))
        , m_bOwnMemory(false)
    { }

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
    { 
        return m_pszString ? m_pszString : ""; 
    }

private:

    void DeAllocateString()
    { }

private:

    char* m_pszString;
    bool m_bOwnMemory;

};

} // !Duckers
