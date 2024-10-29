#include "BeeDebug.hpp"

#include "Crash.hpp"

Bee::Problems::CrashHandling* Bee::Problems::CrashHandling::m_pInstance = new Bee::Problems::CrashHandling();

Bee::Problems::CrashHandling& Bee::Problems::CrashHandling::Get()
{
    return *m_pInstance;
}

void Bee::Problems::CrashHandling::ProtectObject(IDumpOnException* obj)
{
    m_vpObjects.push_back(obj);
}

void Bee::Problems::CrashHandling::Dump()
{
    for (auto& i : m_vpObjects)
        i->Dump();
}
