#pragma once
#include "IEventSubscriber.h"

namespace Fiea::GameEngine
{
	class YoutubeViewer :	public IEventSubscriber
	{
	public:
		/// @brief default constructor and destructor
		YoutubeViewer() = default;
		~YoutubeViewer() = default;

		/// @brief override of IEventSubscriber methods
		/// @param publisher 
		/// @param eventType 
		void Notified(IEvent& publisher, std::string eventType) override;

		/// @brief var for unit testing
		bool watchedVideo = false;
	};
}

