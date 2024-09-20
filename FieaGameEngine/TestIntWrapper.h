#pragma once
#include "IParseHandler.h"

namespace Fiea::GameEngine
{
	/// @brief Wrapper soley for ints, used to test part 1
	class TestIntWrapper : public ParseCoordinator::Wrapper
	{
		RTTI_DECLARATIONS(IntHandler::IntWrapper, ParseCoordinator::Wrapper);
	public:
		/// @brief Default constructor
		/// @param key 
		TestIntWrapper() = default;

		/// @brief constructor with key
		/// @param key 
		//TestIntWrapper(std::string key);

		/// @brief Set the value this wrapper 
		/// @param val 
		void SetInt(int val) { intRef = &val; };

	private:
		/// @brief ref to the int we "hold"
		int* intRef = nullptr;
	};
}

