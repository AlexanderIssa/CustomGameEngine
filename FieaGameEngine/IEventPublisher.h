#pragma once
#include "RTTI.h"
#include <vector>
#include <stdexcept>
//#include "IEvent.h"
//#include "IEventSubscriber.h"


namespace Fiea::GameEngine
{
	#define GameTime size_t	// temp
	
	class IEvent;
	class IEventSubscriber; // forward declaring the class to avoid circular dependency
	// IEventPublisher is like Youtube, who holds all the youtubers and calls notify on each youtuber when they post
	// This is a singleton as there is only one "Youtube"
	class IEventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(IEventPublisher, RTTI);

	public:

		// Static method to get the singleton instance
		static IEventPublisher& CreateInstance() {
			if (instance == nullptr)
			{
				instance = new IEventPublisher();
			}
			else
			{
				throw std::logic_error("TypeReg instance already exists");
			}
			return *instance;
		}

		/// @brief Delete singleton instance
		static void DeleteInstance()
		{
			delete instance;
			instance = nullptr;
		}

		/// @brief Adds an event ref to the events list
		/// @param event
		void Post(IEvent& event);

		/// @brief tells every event in it's list of events to call Notify which calls Notified in all that event's subscribers
		/// @param time 
		void Update(GameTime time);

		/// @brief singleton instance
		inline static IEventPublisher* instance;

	private:
		/// @brief List of events (Youtubers) contained in IEventPublishers (Youtube)
		std::vector<IEvent*> _events;

		// make sure that we don't create an instance outside of here
		IEventPublisher() = default;

		// delete copy const and operator= because we may only have one instance at a time
		IEventPublisher(const IEventPublisher&) = delete;
		IEventPublisher& operator=(const IEventPublisher&) = delete;
	};
}