#pragma once
#include "IEventPublisher.h"
#include "IEventSubscriber.h"

namespace Fiea::GameEngine
{
    // IEvent is like a Youtuber who knows who subscribed to them and notifies all their subscribers when they post a new video
    class IEvent
    {
    public:

        IEvent() = default;

        /// @brief Destructor
        virtual ~IEvent() = default;

        /// @brief This design shouldn't allow for any copying of events since they all share the same info and subs
        IEvent(const IEvent& other) = delete;
        IEvent(const IEvent&& other) = delete;

        /// @brief Iterates through list of subscribers and calls subscriber.Notified()
        virtual void Notify() = 0;

        /// @brief Given the address of an EventSubscriber, add it to the list of subscribers for this event type.
        /// @param sub 
        virtual void AddSubscriber(IEventSubscriber& sub) = 0;

        /// @brief Given the address of an EventSubscriber, remove it from the list of subscribers for this event type.
        /// @param sub
        virtual void RemoveSubscriber(IEventSubscriber& sub) = 0;

        /// @brief Unsubscribe all subscribers to this event type.
        virtual void RemoveAllSubscribers() = 0;

        /// @brief Getter and setter for whether or not this IEvent is a single fire
        bool GetDoOnce() { return doOnce; }
        void SetDoOnce(bool DoOnce) { doOnce = DoOnce; }

        /// @brief takes the current time and returns true if the event has expired (time enqueued + delay)
        /// @param currentTime 
        /// @return bool
        //bool IsExpired(GameTime currentTime);

        /// @brief return the time when this event was enqueued.
        //GameTime TimeEnqueued();

        /// @brief return the amount of time after being enqueued that this event expires.
        /// @return delay time
        GameTime Delay() { return delay; };
        
        /// @brief Set the delay for this event to fire
        /// @param Delay 
        void SetDelay(GameTime Delay) { delay = Delay; }

    protected:

        /// @brief Optional delay time to add to events
        GameTime delay = 0;

        /// @brief bool to determine whether or not the event should be removed from the EventHandler after posting once
        bool doOnce = false;
    };
}

