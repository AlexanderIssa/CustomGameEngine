#include "pch.h"
#include "PlayerObject.h"
#include "Action.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(PlayerObject);

PlayerObject::PlayerObject() : GameObject(PlayerObject::TypeIdClass())
{

}

void PlayerObject::Update(const size_t& time)
{
	PlayerName = "Update hit!";

	// update all child game objects that this game object parents
	for (size_t i = 0; i < _children->Size(); ++i)
	{
		GameObject* s = (*_children)[i].GetScope()->As<GameObject>();
		assert(s != nullptr);

		s->Update(time);
	}

	// update all the actions that this game object parents
	for (size_t i = 0; i < _actions->Size(); ++i)
	{
		Action* temp = (*_actions)[i].GetScope()->As<Action>();
		//Action* temp = _actions->GetScope(i)->As<Action>();
		assert(temp != nullptr);

		temp->Update(time);
	}
}

std::vector<Signature> PlayerObject::Signatures()
{
	std::vector<Signature> parentAttributes = GameObject::Signatures();
	std::vector<Signature> myAttributes = {
		{ "PlayerName"s, Datum::DatumType::String, 1, offsetof(PlayerObject, PlayerName) }
	};

	for (auto sig : parentAttributes)	// for every signature in my parent's attributes, place them into my attributes
	{
		myAttributes.push_back(sig);
	}

	return myAttributes;
}