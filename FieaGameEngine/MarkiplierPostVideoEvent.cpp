#include "pch.h"
#include "MarkiplierPostVideoEvent.h"

using namespace Fiea::GameEngine;


MarkiplierPostVideoEvent::MarkiplierPostVideoEvent()
{
	_subs = new std::vector<IEventSubscriber*>();	// initialize the _subs to point at an empty vector
}

MarkiplierPostVideoEvent::~MarkiplierPostVideoEvent()
{
	delete _subs;	// delete the elements in subs to avoid leaks
}

void MarkiplierPostVideoEvent::Notify()
{
	_payload.posted = true;	// change the payload to display that we posted
	_payload.views += _subs->size();	// change the payload to display the amount of views this post gets

	for (size_t i = 0; i < _subs->size(); ++i)
	{
		(*_subs)[i]->Notified(*this, "MarkiplierPostVideoEvent");	// notify all my subs and tell them who I am
	}
}


void MarkiplierPostVideoEvent::AddSubscriber(IEventSubscriber& sub)
{
	(*_subs).push_back(&sub);
}

void MarkiplierPostVideoEvent::RemoveSubscriber(IEventSubscriber& sub)
{
	for (auto it = (*_subs).begin(); it != (*_subs).end(); ++it)
	{
		if (*it == &sub)
		{
			(*_subs).erase(it);
			break;
		}
	}
}

void MarkiplierPostVideoEvent::RemoveAllSubscribers()
{
	(*_subs).clear();
}