#pragma once
#include "IEventPublisher.h"
//#include "MarkiplierPostVideoEvent.h"

namespace Fiea::GameEngine
{
    class MarkiplierYoutuber :  public IEventPublisher
    {
    public:
        MarkiplierYoutuber() = default;

        ~MarkiplierYoutuber() = default;
    };
}