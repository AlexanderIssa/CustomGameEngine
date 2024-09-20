#pragma once
#include "Action.h"
#include "IFactory.h"

namespace Fiea::GameEngine
{
	class ActionList :	public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);
	public:
		/// @brief default constructor
		ActionList();

		/// @brief constructor for when this ActionList has a child
		/// @param child's id 
		ActionList(size_t id);

		/// @brief Copy constructor
		/// @param other 
		ActionList(const ActionList& other) = default;

		/// @brief deconstructor
		virtual ~ActionList() = default;

		/// @brief Gets the signatures of any Action for attributed
		/// @return vector of signatures
		static std::vector<Signature> Signatures();

		/// @brief Find the target datum and modify it's value by the increment amount
		/// @param time 
		void Update(const GameTime& time) override;

		/// @brief Override for Action SetName
		/// @param name 
		void virtual SetName(std::string name) override;

		/// @brief Override for Action GetName
		/// @return name
		std::string virtual GetName() override { return _name; };

		/// @brief function to create a action and then add that action to the action list
		/// @param actionClass 
		/// @param actionName 
		/// @return a pointer to that created action
		Action* CreateAndAddAction(std::string actionClass, std::string actionName);

		/// @brief Add an action to our actionList
		/// @param actionToAdd 
		void AddToList(Action* actionToAdd, std::string key = "");

		/// @brief Remove the action from our action list
		/// @param key 
		void RemoveActionFromList(const std::string& key);

		/// @brief Get the action with given key from our list, optional index param for arrays of actions
		/// @param key 
		/// @param idx 
		/// @return pointer to the found action
		Action* GetAction(std::string key, size_t idx = 0);

		/// @brief Clone override
		/// @return a copy of this ActionIncrement
		ActionList* Clone() const override;

	protected:
		Scope* _actions;
	};

	MAKE_FACTORY(Action, ActionList);
}