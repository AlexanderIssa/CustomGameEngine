#include "pch.h"
#include "ActionList.h"
#include "FactoryManager.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(ActionList);

ActionList::ActionList()
{
	_actions = new Scope();	// default initialize our actions scope
	Adopt(std::move(_actions), "Actions"s);
}

ActionList::ActionList(size_t id) : Action(id)
{
	_actions = new Scope();	// default initialize our actions scope
	Adopt(std::move(_actions), "Actions"s);
}

std::vector<Signature> ActionList::Signatures()
{
	return std::vector<Signature>{
		{ "Actions"s, Datum::DatumType::Table, 0, offsetof(ActionList, _actions) },
	};
}

void ActionList::Update(const GameTime& time)
{
	for (size_t i = 0; i < _actions->Size(); ++i)
	{
		for (size_t j = 0; j < (*_actions)[i].Size(); ++j)
		{
			Action* temp = (*_actions)[i].GetScope(j)->As<Action>();	// get our nested actions in our actions scope
			if (temp != nullptr)	// if this scope is a game object
			{
				temp->Update(time);	// call update on each nested action
			}
		}
	}
}

void ActionList::SetName(std::string name)
{
	_name = name;
}

Action* ActionList::CreateAndAddAction(std::string actionClass, std::string actionName)
{
	Action* action = FactoryManager<Action>::Create(actionClass);	// create the action
	AddToList(action, actionName);

	return action;
}

void ActionList::AddToList(Action* actionToAdd, std::string key)
{
	if (actionToAdd == nullptr)
	{
		throw std::logic_error("Can't add a non existent action to the list!");
	}

	actionToAdd->SetName(key);	// set the name of the action as the key

	_actions->Adopt(std::move(actionToAdd), actionToAdd->GetName());
}

void ActionList::RemoveActionFromList(const std::string& key)
{
	Datum* d = _actions->Find(key);	// find the action using the key given
	if (d == nullptr)
	{
		throw std::logic_error("Action does not exist in this ActionList");
	}

	Action* a = d->GetScope()->As<Action>();
	if (a == nullptr)
	{
		throw std::logic_error("This Scope is not an action!");
	}
	
	d->Remove(0);	// remove the nested scope action from the datum
}

Action* ActionList::GetAction(std::string key, size_t idx)
{
	Datum* d = _actions->Find(key);	// find the datum holding the action
	if (d == nullptr)
	{
		throw std::logic_error("Action not found!");
	}

	Action* a = d->GetScope()->As<Action>(); // cast it as an action
	if (a == nullptr)
	{
		throw std::logic_error("Scope found is not an action");
	}

	return a;
}

ActionList* ActionList::Clone() const
{
	return FNEW ActionList(*this);
}
