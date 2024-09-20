#pragma once
#include "IEvent.h"

namespace Fiea::GameEngine
{
    class MarkiplierPostVideoEvent :  public IEvent
    {
    public:
        inline static struct MarkPayload
        {
            bool posted = false;
            size_t views = 0;
        } _payload{};

        /// @brief default constructor
        MarkiplierPostVideoEvent();

        /// @brief deconstructor
        ~MarkiplierPostVideoEvent();

        /// @brief Override of function in IEvent
        void Notify() override;

        /// @brief Override of function in IEvent
        /// @param sub 
        void AddSubscriber(IEventSubscriber& sub) override;

        /// @brief Override of function in IEvent
        /// @param sub 
        void RemoveSubscriber(IEventSubscriber& sub) override;

        /// @brief Override of function in IEvent
        void RemoveAllSubscribers() override;

        static MarkPayload& GetPayload() { return _payload; };

    protected:

        /// @brief A vector of subscribers to be notifed when an event happens
        ///        we make it at vector* because unit tests detect for memory leaks before this gets destructed,
        ///        so we must manually delete it outselves.
        ///        It's static so every instance of this event shares the same subscribers.
        inline static std::vector<IEventSubscriber*>* _subs;
    };
}

