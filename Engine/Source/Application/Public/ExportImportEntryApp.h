#pragma once

#include "IApplication.h"


#define EXPORT_APPLICATION(Application)                         \
    ::Duckers::IAppliciaton* ::Duckers::ImportApplication()     \
    { return new Application(); }                               \



namespace Duckers
{

::Duckers::IAppliciaton* ImportApplication();

} // !Duckers
