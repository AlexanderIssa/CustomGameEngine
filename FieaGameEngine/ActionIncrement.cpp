#include "pch.h"
#include "ActionIncrement.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(ActionIncrement);

Fiea::GameEngine::ActionIncrement::ActionIncrement() : Action(ActionIncrement::TypeIdClass())
{

}

std::vector<Signature> ActionIncrement::Signatures()
{
	return std::vector<Signature>{
		{ "Key"s, Datum::DatumType::String, 1, offsetof(ActionIncrement, keyStr) },
		{ "Value"s, Datum::DatumType::Float, 1, offsetof(ActionIncrement, val) }
	};
}

void ActionIncrement::Update(const GameTime& time)
{
	// increment value
	//Datum* temp = Find(keyStr);

	// setup, only runs in the first update call
	if (parentGO == nullptr)
	{
		parentGO = FindParentScopeOfType<GameObject>();
		
		if (parentGO != nullptr)
		{
			size_t temp = 0;
			target = parentGO->FindContainedScope(keyStr, temp);	// find the Datum we want to target that is in our parent GO
			assert(target != nullptr);
		}

		// Didnt work
		//Scope* s = GetParentPtr();
		//GameObject* temp = s->As<GameObject>();

		//if (temp == nullptr)
		//{
		//	Scope* s2 = s->GetParentPtr();
		//	GameObject* temp2 = s2->As<GameObject>();

		//}
		//
		//parentGO = temp;	// set our parent Game Object* to our parent scope
	}

	if (target != nullptr)
	{
		if (target->DatType() == Datum::DatumType::Int)
		{
			target->Get<int32_t>(0) += (int32_t)val;	// increment our int in our int Datum by val
		}
		else if (target->DatType() == Datum::DatumType::Float)
		{
			target->Get<float_t>(0) += val;	// increment our float in our float Datum by val
		}
	}
}

void ActionIncrement::SetName(std::string name)
{
	_name = name;
}

Datum* ActionIncrement::GetTarget()
{
	if (target != nullptr)
	{
		return target;
	}

	if (parentGO == nullptr)
	{
		parentGO = FindParentScopeOfType<GameObject>();

		if (parentGO != nullptr)
		{
			size_t temp = 0;
			target = parentGO->FindContainedScope(keyStr, temp);	// find the Datum we want to target that is in our parent GO
			assert(target != nullptr);
			return target;
		}
		else
		{
			throw std::logic_error("Couldn't find a parent game object!");
		}
	}
	else
	{
		size_t temp = 0;
		target = parentGO->FindContainedScope(keyStr, temp);	// find the Datum we want to target that is in our parent GO
		assert(target != nullptr);
		return target;
	}
}

ActionIncrement* ActionIncrement::Clone() const
{
	return FNEW ActionIncrement(*this);
}
