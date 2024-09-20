#include "pch.h"
#include "IEventPublisher.h"
#include "IEvent.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(IEventPublisher);

void IEventPublisher::Post(IEvent& event)
{
	_events.push_back(&event);	// push back this event into our queue to post on the next update
}

void IEventPublisher::Update(GameTime time)
{
	for (size_t i = 0; i < _events.size(); ++i)	// for every event in our queue
	{
		if (_events[i]->Delay() == 0)	// if there is no delay / delay has ran out
		{
			_events[i]->Notify();	// notify all events in our list
			if (_events[i]->GetDoOnce())	// if the event is a single fire
			{
				// then remove it from our list of events and delete it
				_events.erase(_events.begin() + i);
				--i;	// adjust for the new size of our vector
			}
		}
		else
		{
			GameTime temp = _events[i]->Delay() - 1;	// reduce delay time by one
			_events[i]->SetDelay(temp);	// set this event's delay time decremented
		}
	}
}