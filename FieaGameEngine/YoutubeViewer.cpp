#include "pch.h"
#include "YoutubeViewer.h"
#include "MarkiplierPostVideoEvent.h"

using namespace Fiea::GameEngine;

void YoutubeViewer::Notified(IEvent& publisher, std::string eventType)
{
	if (eventType == "MarkiplierPostVideoEvent")	// if this viewer has watched Markiplier specifically
	{
		watchedVideo = MarkiplierPostVideoEvent::GetPayload().posted;	// If mark published a video then I have watched it
	}
}
