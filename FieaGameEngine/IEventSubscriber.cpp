#include "pch.h"
#include "IEventSubscriber.h"
#include "IEvent.h"

using namespace Fiea::GameEngine;
//using namespace std::string_literals;

void IEventSubscriber::SubscribeTo(IEvent& event)
{
	event.AddSubscriber(*this);
}

void IEventSubscriber::UnsubscribeTo(IEvent& event)
{
	event.RemoveSubscriber(*this);
}
