/// <summary>
/// The declaration of the test parse handler and its wrapper.
/// </summary>

#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"

namespace Fiea::GameEngine
{
	class TestParseHandler final : public IParseHandler {
	public:
		class Wrapper : public ParseCoordinator::Wrapper {
			RTTI_DECLARATIONS(TestParseHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			Wrapper() = default;
			Wrapper(std::string key);

			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;
		};

		//void Initialize() override;
		//void Cleanup() override;
		bool StartHandler(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) override;
		bool EndHandler(const std::string& key, ParseCoordinator::Wrapper* wrapper) override;

		~TestParseHandler() = default;

		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}

