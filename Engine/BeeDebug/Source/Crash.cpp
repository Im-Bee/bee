#include "BeeDebug.hpp"

#include "Crash.hpp"

Bee::Problems::IDumpOnException::IDumpOnException()
{
    Bee::Problems::CrashHandling::Get().AddToList(this);
}

Bee::Problems::IDumpOnException::~IDumpOnException()
{
    Bee::Problems::CrashHandling::Get().Remove(this);
}

Bee::Problems::CrashHandling* Bee::Problems::CrashHandling::m_pInstance = new Bee::Problems::CrashHandling();

Bee::Problems::CrashHandling& Bee::Problems::CrashHandling::Get()
{
    return *m_pInstance;
}

void Bee::Problems::CrashHandling::AddToList(IDumpOnException* obj)
{
    m_vpObjects.push_back(obj);
}

void Bee::Problems::CrashHandling::Remove(IDumpOnException* pObj)
{
    auto iter = m_vpObjects.begin();
    
    while (iter != m_vpObjects.end())
    {
        if (*iter == pObj)
        {
            m_vpObjects.erase(iter);
            return;
        }
    }

    BEE_LOG(Warning, L"void Bee::Problems::CrashHandling::Remove(IDumpOnException*) Couldn't find the pointer.");
}

void Bee::Problems::CrashHandling::Dump()
{
    for (auto& i : m_vpObjects)
        i->Dump();
}

