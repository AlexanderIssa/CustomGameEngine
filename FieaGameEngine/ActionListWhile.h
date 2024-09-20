#pragma once
#include "ActionList.h"
#include "ActionIncrement.h"

namespace Fiea::GameEngine
{
	class ActionListWhile : public ActionList
	{
		RTTI_DECLARATIONS(ActionListWhile, ActionList);
	public:
		/// @brief default constructor
		ActionListWhile();

		/// @brief Copy constructor
		/// @param other 
		ActionListWhile(const ActionListWhile& other) = default;

		/// @brief deconstructor
		~ActionListWhile() override = default;

		/// @brief Gets the signatures of any Action for attributed
		/// @return vector of signatures
		static std::vector<Signature> Signatures();

		/// @brief Find the target datum and modify it's value by the increment amount
		/// @param time 
		void Update(const GameTime& time) override;

		/// @brief Set the condition during runtime
		/// @param cond 
		void SetCondition(int cond);

		/// @brief Set preamble during runtime
		/// @param preamble 
		//void SetPreamble(Action* preamble);

		/// @brief Create an action and set it as this ActionListWhile's Preamble
		/// @param actionClass 
		/// @param actionName 
		/// @return the created Preamble
		Action* CreateAndSetPreamble(std::string actionClass, std::string actionName);

		/// @brief Get Preamble
		/// @return _preamble
		Action* GetPreamble() { return _preamble; }

		/// @brief Set increment during runtime
		/// @param inc 
		//void SetIncrement(Action* inc);

		/// @brief Create an action and set it as this ActionListWhile's Increment
		/// @param actionClass 
		/// @param actionName 
		/// @return the increment created
		Action* CreateAndSetIncrement(std::string actionClass, std::string actionName);

		/// @brief Get Increment
		/// @return _increment
		ActionIncrement* GetIncrement() { return _increment; }

		/// @brief Override for Action SetName
		/// @param name 
		void virtual SetName(std::string name) override;

		/// @brief Override for Action GetName
		/// @return name
		std::string virtual GetName() override { return _name; };

		/// @brief Clone override
		/// @return a copy of this ActionIncrement
		ActionList* Clone() const override;

	protected:
		/// @brief the value used to decide whether to execute or break (execute while non-zero)
		int _condition;

		/// @brief an action to execute prior to executing the first iteration of the loop
		Action* _preamble = nullptr;

		/// @brief an action to execute on completion of each iteration of the loop
		ActionIncrement* _increment = nullptr;

	};
	MAKE_FACTORY(ActionList, ActionListWhile);
}

