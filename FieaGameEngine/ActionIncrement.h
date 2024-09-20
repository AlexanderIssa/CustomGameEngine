#pragma once
#include "Action.h"
#include "Datum.h"
#include "IFactory.h"

namespace Fiea::GameEngine
{
	class ActionIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);
	public:
		/// @brief default constructor
		ActionIncrement();

		/// @brief Copy constructor
		/// @param other 
		ActionIncrement(const ActionIncrement& other) = default;

		/// @brief deconstructor
		virtual ~ActionIncrement() = default;

		/// @brief Gets the signatures of any Action for attributed
		/// @return vector of signatures
		static std::vector<Signature> Signatures();

		/// @brief Find the target datum and modify it's value by the increment amount
		/// @param time 
		void Update(const GameTime& time) override;

		/// @brief Override for Action SetName
		/// @param name 
		void virtual SetName(std::string name) override;

		/// @brief helper to set our key without JSON
		/// @param key 
		void SetKey(std::string key) { keyStr = key; };

		/// @brief Set the val of this action
		/// @param new Val 
		void SetValue(float_t nVal) { val = nVal; };

		/// @brief Override for Action GetName
		/// @return name
		std::string virtual GetName() override { return _name; };

		Datum* GetTarget();
		
		/// @brief Clone override
		/// @return a copy of this ActionIncrement
		ActionIncrement* Clone() const override;

	private:
		/// @brief a prescribed attribute for the key to a datum on the parent gameobject to update
		std::string keyStr = "";
		
		/// @brief a prescribed attribute, that holds a value to add to the target
		float_t val = 1;

		/// @brief our Datum Target to increment the value of
		Datum* target = nullptr;
	};

	MAKE_FACTORY(Action, ActionIncrement);
}

