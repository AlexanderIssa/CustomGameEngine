#pragma once
#include "Attributed.h"
#include "GameObject.h"

namespace Fiea::GameEngine
{
	#define GameTime size_t	// temp

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);
	public:
		/// @brief default constructor
		Action();
		
		/// @brief default deconstructor
		~Action() = default;

		/// @brief copy constructor
		/// @param other 
		Action(const Action& other) = default;

		/// @brief move constructor
		/// @param other 
		Action(Action&& other) = default;

		/// @brief Virtual update function for any Action
		/// @param time 
		void virtual Update(const GameTime& time) = 0;

		/// @brief Sets the name of the action
		/// @param name 
		void virtual SetName(std::string name) = 0;

		/// @brief Gets the name of the action
		/// @return name
		std::string virtual GetName() = 0;

		/// @brief Gets the signatures of any Action for attributed
		/// @return vector of signatures
		static std::vector<Signature> Signatures();

		/// @brief Clones an action
		/// @return a cloned action
		Action* Clone() const override = 0;

	protected:
		/// @brief Constructor to be called by children
		/// @param child's id 
		Action(size_t id);

		/// @brief name of this action
		std::string _name = "";

		/// @brief Gameobject that owns this action
		GameObject* parentGO = nullptr;
	};
}


