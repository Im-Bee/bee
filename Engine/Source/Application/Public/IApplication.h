#pragma once

namespace Duckers
{

class IAppliciaton
{
public:

    IAppliciaton() = default;

    virtual ~IAppliciaton() 
    { }

public:

    virtual void Initialize() = 0;

    virtual void Update() = 0;

    virtual void Render() = 0;

    virtual void Destroy() = 0;

};

} // !Duckers
