#include "BeeUtil.hpp"

#include "Memory/String.hpp"

float Bee::Utils::Memory::StoFC(const char*      pBuff, 
                                const b_uintmem& uBuffSize, 
                                      b_uintmem& cI)
{
    float r = 0.0f;
    float a = 10.0f;
    bool bIsAfterDot = false;

    for (cI; cI < uBuffSize; ++cI)
    {
        const auto& c = pBuff[cI];

        if (c == ' ' ||
            c == '\r' ||
            c == '\n')
        {
            return r;
        }

        if (c == '.')
        {
            a = 0.1f;
            bIsAfterDot = true;
            continue;
        }

        if (c < '0' || c > '9')
        {
            continue;
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

    return BEE_INFINITY;
}

char Bee::Utils::Memory::ToLower(char c)
{
    if (c < 'a')
    {
        c += 0x20;
    }
    else
    {
        return c;
    }
}
