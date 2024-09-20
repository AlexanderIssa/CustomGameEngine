#pragma once

#include "json/json.h"	// https://www.youtube.com/watch?v=OYQDnV092hI
#include "RTTI.h"
#include <map>
#include <memory>

using std::unique_ptr;

namespace Fiea::GameEngine
{
	class IParseHandler;

	class ParseCoordinator
	{
	public:

		class Wrapper : public RTTI	// This class will represent data that all helpers share with each other and with the coordinator. 
		{
			RTTI_DECLARATIONS(ParseCoordinator::Wrapper, RTTI);
		public:
			/// @brief increment a “nesting depth” counter. This gets incremented upon the start of each element.
			void IncrementDepth();

			/// @brief decrement a “nesting depth” counter. This gets decremented upon the end of each element.
			void DecrementDepth();

			/// @brief return _depth
			int Depth() { return _depth; };

		private:
			/// @brief current depth of this wrapper
			int _depth = 0;
		};

		/// @brief dont need a default constructor that doesn't take a wrapper, I ALWAYS need a wrapper for my handler
		ParseCoordinator() = delete;

		/// @brief	given a reference to a Wrapper object, initialize this object.
		/// @param  wrapper
		ParseCoordinator(Wrapper& wrapper);

		/// @brief Destructor
		~ParseCoordinator() = default;

		/// @brief given a reference to an IParseHelper object, add it to the list.
		/// @param helper
		size_t AddHandler(unique_ptr<IParseHandler>&& handler);

		/// @brief given a reference to an IParseHelper object, remove it from the list.
		/// @param helper 
		void RemoveHelper(IParseHandler& handler);

		/// @brief Deserialize JSON string 
		/// @param json
		bool DeserializeObject(std::string json);

		/// @brief given a filename, read in the file and parse it.
		/// @param filename
		bool DeserialzeObjectFromFile(std::string filename);

		/// @brief Deserialize object using string stream
		/// @param object 
		bool DeserializeObject(std::istream object);

		/// @brief return the address of the Wrapper associated with this object.
		Wrapper& GetWrapper() { return *currWrapper; };

	private:
		/// @brief Retrieves the members from a Json::Value object and Parse's each of them.
		/// @params obj
		bool ParseMembers(Json::Value obj);

		/// @brief	trigger the chain of responsibility.
		///			walk through the list of helpers and invoke the StartHander methods until one responds that it has handled the pair.
		///			The handler that responded true to a StartHandler call should be the same handler that accepts the EndHandler call.
		///			If the value is an object or an array of objects to descend (i.e. recursively parse) into the child name/value pairs.
		/// @param	key
		/// @param	obj
		/// @param	isArray
		bool Parse(std::string key, Json::Value obj);

		/// @brief used to add multiple handlers in a map
		size_t _nextRef = 0;

		/// @brief Pointer to hold the address of the wrapper passed in by the user
		Wrapper* currWrapper = nullptr;

		/// @brief map that holds all pointers to our handlers
		std::map<size_t, unique_ptr<IParseHandler>> _handlers;
	};
}
