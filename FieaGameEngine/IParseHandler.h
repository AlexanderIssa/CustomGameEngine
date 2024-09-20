#pragma once
#include "ParseCoordinator.h"
#include <json/value.h>


namespace Fiea::GameEngine
{
	class IParseHandler
	{
	public:
		/// @brief This will get called just before each file is parsed... allows the helper to initialize state, if necessary
		//virtual void Initialize() {}
		
		/// @brief (pure virtual) Given a wrapper reference, a string for the Json key,
		///			a reference to the Json::Value object, and a bool indicating if the value is an array element, attempt to
		///			handle the key/value pair. If this routine does indeed handle the pair, return true, otherwise return false.
		/// @param key 
		/// @param jsonObj 
		/// @param isArray 
		/// @param wrapper
		/// @return bool
		virtual bool StartHandler(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) = 0;

		/// @brief (pure virtual) Given a wrapper reference, a string for the Json key,
		///		   attempt to complete the handling of the element pair. If this routine does indeed handle the pair, return true, otherwise return false. 
		/// @param key 
		/// @param wrapper
		/// @return bool
		virtual bool EndHandler(const std::string& key, ParseCoordinator::Wrapper* wrapper) = 0;

		/// @brief Cleanup function
		//virtual void Cleanup();

		/// @brief Virutal destructor
		virtual ~IParseHandler() = default;

	};
}