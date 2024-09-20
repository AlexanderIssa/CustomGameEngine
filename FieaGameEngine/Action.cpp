#include "pch.h"
#include "Action.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(Action);

Action::Action() : Attributed (Action::TypeIdClass())
{

}

Action::Action(size_t id) : Attributed(id)
{

}

std::vector<Signature> Action::Signatures()
{
	return std::vector<Signature>{
		{ "Name"s, Datum::DatumType::String, 1, offsetof(Action, _name) }
	};
}
