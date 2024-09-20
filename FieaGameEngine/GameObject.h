#pragma once
#include "FnewMacro.h"
#include "Attributed.h"
#include "Signature.h"
#include "IFactory.h"

namespace Fiea::GameEngine
{
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public:
		/// @brief Default constructor, sends ID to attributed
		GameObject();

		/// @brief Child's overload, this constructor will only be called if this GameObject has a child
		/// @param id 
		GameObject(size_t id);

		/// @brief Destructor
		virtual ~GameObject() = default;

		/// @brief copy constructor
		/// @param other 
		GameObject(const GameObject& other) = default;

		/// @brief get this game object's signatures
		/// @return vector<Signature>
		static std::vector<Signature> Signatures();

		/// @brief Update method that runs every tick
		/// @param time 
		virtual void Update(const size_t& time);

		/// @brief takes a class name and an instance name to instantiate a new Action object and adopt it into the Actions datum.
		/// @param className 
		/// @param instanceName 
		virtual void CreateAction(std::string className, std::string instanceName);

		/// @brief Getter to get specific actions from our list of actions
		/// @param actionName 
		/// @return that specific datum as an action
		virtual Scope* GetAction(const std::string& actionName);
		
		/// @brief adopt a GameObject child in the scope level
		/// @param child , childName
		virtual void AddChild(Scope* child, std::string childName = "");

		virtual void RemoveChild(const std::string& childName);

		/// @brief Get child by name
		/// @param child 
		virtual const Datum* GetChild(const std::string& child) const;
		virtual Datum* GetChild(const std::string& child);

		/// @brief getter for returning the Actions datum
		/// @return Actions Datum
		virtual const Scope* Actions() { return _actions; };

		GameObject* Clone() const override;

		std::string name = "";

		struct Transform
		{
			glm::vec4 pos;
			glm::vec4 rot;
			glm::vec4 scale;
		};

		Transform transform;

	protected:
		Scope* _actions;
		Scope* _children;
	};

	MAKE_FACTORY(Scope, GameObject);
}