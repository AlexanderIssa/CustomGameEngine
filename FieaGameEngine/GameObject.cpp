#include "pch.h"
#include "GameObject.h"
#include "Action.h"
#include "FactoryManager.h"
#include "ActionListWhile.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(GameObject);

GameObject::GameObject() : Attributed(GameObject::TypeIdClass())
{
	_actions = FNEW Scope();
	_children = FNEW Scope();

	Adopt(std::move(_actions), "Actions"s);
	Adopt(std::move(_children), "Children"s);
}

GameObject::GameObject(size_t id) : Attributed(id)
{
	_actions = FNEW Scope();
	_children = FNEW Scope();

	Adopt(std::move(_actions), "Actions"s);
	Adopt(std::move(_children), "Children"s);
}

std::vector<Signature> GameObject::Signatures()
{
	return std::vector<Signature>
	{
		{ "name"s, Datum::DatumType::String, 1, offsetof(GameObject, name) },
		{ "TransformPos"s, Datum::DatumType::Vec4, 1, offsetof(GameObject, transform.pos) },
		{ "TransformRot"s, Datum::DatumType::Vec4, 1, offsetof(GameObject, transform.rot) },
		{ "TransformScale"s, Datum::DatumType::Vec4, 1, offsetof(GameObject, transform.scale) },
		{ "Actions"s, Datum::DatumType::Table, 1, offsetof(GameObject, _actions) }
	};
}

void Fiea::GameEngine::GameObject::Update(const size_t& time)
{
	// update all child game objects that this game object parents
	for (size_t i = 0; i < _children->Size(); ++i)
	{
		GameObject* s = (*_children)[i].GetScope()->As<GameObject>();
		assert(s != nullptr);

		s->Update(time);
	}

	//std::vector<Datum*> temp = GetDatums();

	//for (const auto& datum : temp)
	//{
	//	if (datum->_datType == Datum::DatumType::Table)
	//	{
	//		Scope* tempScope = datum->GetScope();

	//		GameObject* tempGO = tempScope->As<GameObject>();
	//		if (tempGO == nullptr)
	//		{
	//			break;	// this Datum holds a scope that is not a Game Object
	//		}
	//		tempGO->Update(time);
	//	}
	//}

	// update all the actions that this game object parents
	for (size_t i = 0; i < _actions->Size(); ++i)
	{
		Action* temp = (*_actions)[i].GetScope()->As<Action>();
		//Action* temp = _actions->GetScope(i)->As<Action>();
		assert(temp != nullptr);

		temp->Update(time);	
	}

}

// this method must be updated everytime a new Action subclass gets created!
void GameObject::CreateAction(std::string className, std::string instanceName)
{
	if (className == "ActionIncrement")
	{
		Scope* temp = FactoryManager<Action>::Create("ActionIncrement");	// create the action subclass object we want
		temp->As<Action>()->SetName(instanceName);

		//_actions->GetScope()->Adopt(std::move(temp), instanceName);	
		//_actions->Adopt(std::move(temp), instanceName);	// adopt it as a child of this GameObject using the _actions Scope
		_actions->Adopt(std::move(temp), instanceName, this);	// adopt it as a child of this GameObject using the _actions Scope
	}
	else if (className == "ActionList")
	{
		Scope* temp = FactoryManager<Action>::Create("ActionList");	// create the action subclass object we want
		temp->As<Action>()->SetName(instanceName);

		//_actions->Adopt(std::move(temp), instanceName);	// adopt it as a child of this GameObject using the _actions Scope
		_actions->Adopt(std::move(temp), instanceName, this);	// adopt it as a child of this GameObject using the _actions Scope
	}
	else if (className == "ActionListWhile")
	{
		Scope* temp = FactoryManager<ActionList>::Create("ActionListWhile");	// create the action subclass object we want
		temp->As<ActionList>()->SetName(instanceName);

		//_actions->Adopt(std::move(temp), instanceName);	// adopt it as a child of this GameObject using the _actions Scope
		_actions->Adopt(std::move(temp), instanceName, this);	// adopt it as a child of this GameObject using the _actions Scope
	}
	else
	{
		throw std::logic_error("There is no Action subclass of given Class Name!");
	}
}

Scope* GameObject::GetAction(const std::string& actionName)
{
	Datum* d = _actions->Find(actionName);	// find the action in our _actions
	if (d == nullptr)
	{
		throw std::logic_error("No such action exsits");
	}

	return d->GetScope();	// return that datum's scope
}


void GameObject::AddChild(Scope* child, std::string childName)
{
	GameObject* temp = child->As<GameObject>();
	assert(temp != nullptr);	// child passed in must be a GameObject

	if (childName != "")	// if we have a passed in name
	{
		// adopt this child game object in our current game object
		// but we want its parent to be this GameObject and not the _children scope
		_children->Adopt(std::move(child), childName, this);
	}
	else
	{
		// if we don't then just name this child the default name
		_children->Adopt(std::move(child), temp->name, this);
	}
}

void Fiea::GameEngine::GameObject::RemoveChild(const std::string& childName)
{
	Datum* tempChild = _children->Find(childName);
	if (tempChild != nullptr)
	{
		//tempChild->GetScope()->Orphan();
		_children->DisownChild(tempChild, childName);
	}
	else
	{
		throw std::logic_error("Trying to remove a child that doesnt exist!");
	}
}

const Datum* GameObject::GetChild(const std::string& child) const
{
	return _children->Find(child);	// check to see if we have a child by that name and if we do then return a pointer to it's datum, if not, null
}

Datum* GameObject::GetChild(const std::string& child)
{
	return _children->Find(child);	// check to see if we have a child by that name and if we do then return a pointer to it's datum, if not, null
}

GameObject* GameObject::Clone() const
{
	return FNEW GameObject(*this);
}
