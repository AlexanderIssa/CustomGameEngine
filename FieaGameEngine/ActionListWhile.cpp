#include "pch.h"
#include "ActionListWhile.h"
#include "FactoryManager.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(ActionListWhile);

ActionListWhile::ActionListWhile() : ActionList(ActionListWhile::TypeIdClass())
{

}

std::vector<Signature> ActionListWhile::Signatures()
{
	return std::vector<Signature>{
		{ "Condition"s, Datum::DatumType::Int, 1, offsetof(ActionListWhile, _condition) },
		{ "Preamble"s, Datum::DatumType::Table, 0, offsetof(ActionListWhile, _preamble) },
		{ "Increment"s, Datum::DatumType::Table, 0, offsetof(ActionListWhile, _increment) },
	};
}

void ActionListWhile::Update(const GameTime& time)
{
	// Execute preamble (if present)
	if (_preamble != nullptr)
	{
		_preamble->Update(time);
	}
	
	// if we have an increment action, we use a while loop that takes that into consideration
	if (_increment != nullptr)
	{
		_condition = _increment->GetTarget()->Get<int32_t>();
		while (_condition > 0)
		{
			for (size_t i = 0; i < _actions->Size(); ++i)
			{
				Action* temp = (*_actions)[i].GetScope()->As<Action>();	// get our nested actions in our actions scope
				assert(temp != nullptr);

				temp->Update(time);	// call update on each nested action
			}

			// followed by the increment action
			_increment->Update(time);
			_condition = _increment->GetTarget()->Get<int32_t>();
		}
	}
	else
	{
		// then Loop until condition becomes zero calling all actions in List
		while (_condition > 0)
		{
			for (size_t i = 0; i < _actions->Size(); ++i)
			{
				Action* temp = (*_actions)[i].GetScope()->As<Action>();	// get our nested actions in our actions scope
				assert(temp != nullptr);

				temp->Update(time);	// call update on each nested action
			}

			_condition--;	// decrement condition so there is no infinite loop
		}
	}
}

void ActionListWhile::SetCondition(int cond)
{
	_condition = cond;
}

//void ActionListWhile::SetPreamble(Action* preamble)
//{
//	assert(preamble != nullptr);
//
//	Datum* d = _actions->Find(preamble->GetName());	// is this action part of our actions list?
//	if (d != nullptr)
//	{
//		// if it is then remove it from the list and only keep it as our preamble
//		RemoveActionFromList(preamble->GetName());
//	}
//
//	_preamble = preamble;
//	Adopt(std::move(_preamble), "Preamble"s);	// adopt this action as our child
//}

Action* ActionListWhile::CreateAndSetPreamble(std::string actionClass, std::string actionName)
{
	_preamble = FactoryManager<Action>::Create(actionClass);	// create the action and set it as the Preamble

	Adopt(std::move(_preamble), "Preamble"s);	// adopt this action as our child
	return _preamble;
}

//void ActionListWhile::SetIncrement(Action* inc)
//{
//	assert(inc != nullptr);
//
//	Datum* d = _actions->Find(inc->GetName());	// is this action part of our actions list?
//	if (d != nullptr)
//	{
//		// if it is then remove it from the list and only keep it as our increment
//		RemoveActionFromList(inc->GetName());
//	}
//	
//	ActionIncrement* temp = inc->As<ActionIncrement>();
//	if (temp == nullptr)
//	{
//		throw std::logic_error("this action is not an ActionIncrement!");
//	}
//
//	_increment = temp;
//	Adopt(std::move(_increment), "Increment"s);	// adopt this action as our child
//}

Action* ActionListWhile::CreateAndSetIncrement(std::string actionClass, std::string actionName)
{
	Action* a = FactoryManager<Action>::Create(actionClass);	// create the action
	_increment = a->As<ActionIncrement>();
	
	if (_increment == nullptr)
	{
		throw std::logic_error("Didn't pass in an ActionIncrement!");
	}
	Adopt(std::move(_increment), "Increment"s);	// adopt this action as our child
	return _increment;
}

void ActionListWhile::SetName(std::string name)
{
	_name = name;
}

ActionList* Fiea::GameEngine::ActionListWhile::Clone() const
{
	return FNEW ActionListWhile(*this);
}
