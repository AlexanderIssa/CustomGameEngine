#include "pch.h"
#include "TestIntHandler.h"

namespace Fiea::GameEngine
{
	bool TestIntHandler::StartHandler(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
	{
		if (value.isInt())	// handler can only handle ints
		{
			TestIntWrapper* intWrapper = wrapper->As<TestIntWrapper>();
			if (intWrapper == nullptr) return false;	// if we cant cast this wrapper to an int wrapper then we got sent the wrong one

			int val = value.asInt();
			intWrapper->SetInt(val);	// int wrapper now points at this val
			return true;
		}
		return false;	// not an int
	}

	bool TestIntHandler::EndHandler(const std::string& key, ParseCoordinator::Wrapper* wrapper)
	{
		return false;
	}

	bool TestIntHandler::Equals(const RTTI* rhs) const
	{
		return false;
	}

	std::string TestIntHandler::ToString() const
	{
		return std::string();
	}
}
