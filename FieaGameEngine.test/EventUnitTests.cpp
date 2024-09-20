#include "pch.h"
#include "CppUnitTest.h"
#include "IEventSubscriber.h"
#include "IEvent.h"
#include "IEventPublisher.h"
#include "YoutubeViewer.h"
#include "MarkiplierYoutuber.h"
#include "MarkiplierPostVideoEvent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(EventsTest)
	{
		TEST_METHOD(EventUpdate)
		{
			YoutubeViewer Alex;	// Event listeners, or youtube viewers
			YoutubeViewer Jeff;
			MarkiplierPostVideoEvent markVidPost;	// event itself, where markiplier posts a video

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 0);	// every instance of MarkiplierPostVideoEvent has the same payload
			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().posted == false);	// mark hasn't posted the video yet

			Alex.SubscribeTo(markVidPost);	// Alex subscribes to any video Markiplier posts on youtube
			IEventPublisher::instance->Post(markVidPost);	// Markiplier has posted a new video
			IEventPublisher::instance->Update(1);	// time moves

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 1);	// views went up by one since Alex has watched it
			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().posted == true);	// mark has posted his video
			Assert::IsTrue(Alex.watchedVideo == true);	// Alex has watched the new Markiplier youtube video!
			Assert::IsTrue(Jeff.watchedVideo == false);	// but Jeff hasn't! Because Jeff hasn't subscribed to Markiplier yet (silly guy)

			Jeff.SubscribeTo(markVidPost);	// jeff subscribes to Markiplier

			IEventPublisher::instance->Update(1);	// time moves
			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 3);	// Alex and Jeff have watched the video! View count goes up by 2
			Assert::IsTrue(Alex.watchedVideo == true);	// Alex is still watching the same video
			Assert::IsTrue(Jeff.watchedVideo == true);	// Jeff has also watched the new Markiplier youtube video!

			Alex.watchedVideo = false; // reset Alex
			Alex.UnsubscribeTo(markVidPost);	// unsub to markiplier :(

			IEventPublisher::instance->Update(1);	// time moves

			Assert::IsTrue(Alex.watchedVideo == false);	// since Alex is unsubbed to markiplier, Alex has NOT watched the new video (fake fan)
			Assert::IsTrue(Jeff.watchedVideo == true);	// since Alex is unsubbed to markiplier, Alex has NOT watched the new video (fake fan)
			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 4);	// since Alex is unsubbed to markiplier but Jeff still is, the view count only goes up by 1

			markVidPost.RemoveAllSubscribers();	// Markiplier has quit youtube, so now all his subscribers are gone :(

			IEventPublisher::instance->Update(1);	// time moves
			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 4);	// since he has no more subs, his view count is forever the same

			// reset data
			MarkiplierPostVideoEvent::GetPayload().posted = false;
			MarkiplierPostVideoEvent::GetPayload().views = 0;
		}

		TEST_METHOD(EventDoOnce)
		{
			YoutubeViewer Kyle;	// Event listeners, or youtube viewers
			YoutubeViewer Mark;
			MarkiplierPostVideoEvent markVidPost2;	// event itself, where markiplier posts a video

			markVidPost2.SetDoOnce(true);	// once markiplier publishes the video he will instantly take it down
			Assert::IsTrue(markVidPost2.GetDoOnce());	// Do once has been set to true
			IEventPublisher::instance->Post(markVidPost2);	// Markiplier has posted the video

			Kyle.SubscribeTo(markVidPost2);	// Kyle and Mark subscribe to Markiplier
			Mark.SubscribeTo(markVidPost2);

			IEventPublisher::instance->Update(1);	// time moves

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 2);	// the video recieves 2 views

			IEventPublisher::instance->Update(1);	// time moves

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 2);	// since markiplier took the video down, the video is stuck at 2 views

			// reset data
			MarkiplierPostVideoEvent::GetPayload().posted = false;
			MarkiplierPostVideoEvent::GetPayload().views = 0;
		}

		TEST_METHOD(EventDelay)
		{
			YoutubeViewer Marissa;	// Event listeners, or youtube viewers
			YoutubeViewer Jordan;
			MarkiplierPostVideoEvent markVidPost3;	// event itself, where markiplier posts a video

			markVidPost3.SetDelay(3);	// Markiplier is waiting for the video to upload which will take 3 + 1 "GameTime"s
			Assert::IsTrue(markVidPost3.Delay() == 3);	// Delay is 3
			IEventPublisher::instance->Post(markVidPost3);	// Markiplier has posted the video, but it wont upload until delay is finished

			Marissa.SubscribeTo(markVidPost3);	// Marissa and Jordan subscribe to Markiplier
			Jordan.SubscribeTo(markVidPost3);

			IEventPublisher::instance->Update(1);	// time moves by 1 "GameTime"

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 0);	// He has recieved no views as the video is still uploading
			Assert::IsFalse(MarkiplierPostVideoEvent::GetPayload().posted);	// Video is not posted yet

			IEventPublisher::instance->Update(1);	// time moves

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 0);	// Still uploading...
			Assert::IsFalse(MarkiplierPostVideoEvent::GetPayload().posted);

			IEventPublisher::instance->Update(1);	// time moves

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 0);	// Still uploading...he has slow internet
			Assert::IsFalse(MarkiplierPostVideoEvent::GetPayload().posted);

			IEventPublisher::instance->Update(1);	// time moves

			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().views == 2);	// It's posted! Mark instantly gains 2 views from Marissa and Jordan!
			Assert::IsTrue(MarkiplierPostVideoEvent::GetPayload().posted);

			// reset data
			MarkiplierPostVideoEvent::GetPayload().posted = false;
			MarkiplierPostVideoEvent::GetPayload().views = 0;
		}

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			// create singleton instance
			IEventPublisher::CreateInstance();
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			// delete singleton instance
			IEventPublisher::instance->DeleteInstance();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

	private:
		inline static _CrtMemState _startMemState;	// in addition to declaring it, inline also gives storage for it as well
	};
}