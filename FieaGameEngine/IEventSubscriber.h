#pragma once
#include <functional>
#include <string>
//#include "IEventPublisher.h"
//#include "IEvent.h"

namespace Fiea::GameEngine
{
	// Event subscriber is like a youtube viewer who subscribes to Youtubers (IEvents)
	class IEvent;
	class IEventSubscriber
	{
	public:

		/// @brief Default constructor
		IEventSubscriber() = default;

		/// @brief virtual destructor
		virtual ~IEventSubscriber() = default;

		/// @brief pure virtual method that accepts the address of an EventPublisher (the actual argument will be the event itself)
		///		   The handlers will use the RTTI interface to verify actual the event type.
		/// @param publisher 
		virtual void Notified(IEvent& publisher, std::string eventType) = 0;

		/// @brief Subscribe to a specific Youtuber (event)
		/// @param event 
		void SubscribeTo(IEvent& event);

		/// @brief UnSubscribed to a specific Youtuber (event)
		/// @param event 
		void UnsubscribeTo(IEvent& event);

	protected:

	};
}

