#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "TestIntWrapper.h"

namespace Fiea::GameEngine
{
	/// @brief Handler for ints
	class TestIntHandler final : public IParseHandler 
	{
	public:
		bool StartHandler(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) override;
		bool EndHandler(const std::string& key, ParseCoordinator::Wrapper* wrapper) override;

		~TestIntHandler() = default;

		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}


