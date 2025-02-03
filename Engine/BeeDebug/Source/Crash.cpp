#include "BeeDebug.hpp"

#include <list>

using namespace std;
using namespace Bee::Debug;

list<IOnException*> _vpOnExceptObjects = list<IOnException*>();

CrashHandler* CrashHandler::m_pInstance = new CrashHandler();

IOnException::IOnException()
{
    CrashHandler::Get().Attach(this);
}

IOnException::~IOnException()
{
    CrashHandler::Get().Detach(this);
}

CrashHandler& CrashHandler::Get()
{
    return *m_pInstance;
}

void CrashHandler::Attach(IOnException* obj)
{
    _vpOnExceptObjects.push_back(obj);
}

void CrashHandler::Detach(IOnException* pObj)
{
    auto iter = _vpOnExceptObjects.begin();
    
    while (iter != _vpOnExceptObjects.end())
    {
        if (*(iter++) == pObj)
        {
            _vpOnExceptObjects.erase(iter);
            return;
        }
    }

    BEE_LOG(Warning, L"void CrashHandler::Detach(IOnException*): Couldn't find pObj.");
}

void CrashHandler::HandleObjects()
{
    for (auto& i : _vpOnExceptObjects)
    {
        i->HandleObjects();
    }
}

