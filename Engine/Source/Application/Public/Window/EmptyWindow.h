#include "Window.h"

namespace Duckers
{

class DUCKERS_API EmptyWindow final : public Window
{
public:

    void HandleOtherEvents(int32) override final
    { }

};

}
