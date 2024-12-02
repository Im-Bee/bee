#include "BeeDebug.hpp"

#include <list>


using namespace Bee::Problems;

std::list<IDumpOnException*> _vpObjects = std::list<IDumpOnException*>();


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
    _vpObjects.push_back(obj);
}

void Bee::Problems::CrashHandling::Remove(IDumpOnException* pObj)
{
    auto iter = _vpObjects.begin();
    
    while (iter != _vpObjects.end())
    {
        if (*(iter++) == pObj)
        {
            _vpObjects.erase(iter);
            return;
        }
    }

    BEE_LOG(Warning, L"void Bee::Problems::CrashHandling::Remove(IDumpOnException*) Couldn't find the pointer.");
}

void Bee::Problems::CrashHandling::Dump()
{
    for (auto& i : _vpObjects)
        i->Dump();
}

