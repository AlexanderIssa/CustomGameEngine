#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "Scope.h"

namespace Fiea::GameEngine
{
	class ScopeHandler : public IParseHandler
	{
	public:
		class ScopeWrapper : public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(ScopeHandler::ScopeWrapper, ParseCoordinator::Wrapper);

		public:
			/// @brief delete the default scope wrapper constructor, we never use a wrapper without a scope ref
			ScopeWrapper() = delete;

			/// @brief The "default" constructor, we always want to pass in a scope ref
			/// @param scopeRef 
			ScopeWrapper(Scope& scopeRef);

			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			/// @brief RTTI Equals()
			/// @param rhs 
			/// @return bool
			bool Equals(const RTTI* rhs) const;

			/// @brief RTTI ToString()
			/// @return 
			std::string ToString() const;

			/// @brief Set the value this wrapper 
			/// @param val 
			//void SetInt(int val) { intRef = &val; };

			/// @brief return scopePtr
			/// @return scopePtr
			Scope* GetCurrentScope() { return _currScopePtr; };

			/// @brief Set our current scope this wrapper is pointing at, used for nested scopes
			/// @param scope 
			void SetCurrentScope(Scope& scope) { _currScopePtr = &scope; }

			/// @brief Sets our current scope* to point at the current scope's parent
			void SetCurrentScopeToParent() { _currScopePtr = &_currScopePtr->GetParent(); }

		private:
			/// @brief this is the root scope we point to
			Scope* _rootScopePtr;

			/// @brief This is the current scope we are pointing to
			Scope* _currScopePtr;
		};

		/// @brief override of IParseHandler StartHandler()
		/// @param key 
		/// @param value 
		/// @param wrapper 
		/// @return bool
		bool StartHandler(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) override;

		/// @brief override of IParseHandler EndHandler()
		/// @param key 
		/// @param wrapper 
		/// @return bool
		bool EndHandler(const std::string& key, ParseCoordinator::Wrapper* wrapper) override;

		/// @brief Default deconstructor
		~ScopeHandler() = default;

		/// @brief (Unimplemented) RTTI equals
		/// @param rhs 
		/// @return bool
		bool Equals(const RTTI* rhs) const;

		/// @brief (Unimplemented) RTTI ToString
		/// @return string
		std::string ToString() const;

		//bool initializeCalled{ false };
		//bool cleanupCalled{ false };
		
		/// @brief Count trackers
		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}

