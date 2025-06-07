#include "BeeUtil.hpp"

#include "Memory/String.hpp"

float Bee::Utils::Memory::StoFC(const char*    pBuff, 
                                const b_usize& uBuffSize, 
                                      b_usize* cI)
{
    float r = 0.0f;
    float a = 10.0f;
    bool bIsAfterDot = false;
    bool bNegative = false;
    
    b_usize* localCI;
    if (cI)
    {
        localCI = cI;
    }
    else
    {
        localCI = new b_usize(0);
    }

    for (; *localCI < uBuffSize; ++(*localCI))
    {
        const auto& c = pBuff[*localCI];

        if (c == ' '  ||
            c == '\r' ||
            c == '\n')
        {
            break;
        }

        if (c == '-')
        {
            bNegative = true;
            continue;
        }

        if (c == '.')
        {
            a = 0.1f;
            bIsAfterDot = true;
            continue;
        }

        if (c < '0' || c > '9')
        {
            return r;
        }

        if (bIsAfterDot)
        {
            r = r + ((c - '0') * a);
            a *= 0.1f;
        }
        else
        {
            r = r * a + (c - '0');
        }
    }

    if (bNegative)
    {
        r *= -1.f;
    }

    if (!cI)
    {
        delete localCI;
    }

    return r;
}

int Bee::Utils::Memory::StoIC(const char*    pBuff, 
                              const b_usize& uBuffSize, 
                                    b_usize* cI)
{
    int r = 0;
    bool bNegative = false;

    b_usize* localCI;
    if (cI)
    {
        localCI = cI;
    }
    else
    {
        localCI = new b_usize(0);
    }

    for (; *localCI < uBuffSize; ++(*localCI))
    {
        const auto& c = pBuff[*localCI];

        if (c == ' ' ||
            c == '\r' ||
            c == '\n')
        {  
            break;
        }

        if (c == '-')
        {
            bNegative = true;
        }

        if (c < '0' || c > '9')
        {
            return r;
        }

        r = r * 10 + (c - '0');
    }

    if (bNegative)
    {
        r *= -1;
    }

    if (!cI)
    {
        delete localCI;
    }

    return r;
}

char Bee::Utils::Memory::ToLower(char c)
{
    if (c >= 'A' && c < 'a')
    {
        c += 0x20;
    }

    return c;
}
