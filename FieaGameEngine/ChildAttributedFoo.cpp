#include "pch.h"
#include "ChildAttributedFoo.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(ChildAttributedFoo);

// Default constructor
ChildAttributedFoo::ChildAttributedFoo() : AttributedFoo(ChildAttributedFoo::TypeIdClass())
{
	//if (TypeRegistry::instance != nullptr)
	//{
	//	TypeRegistry::instance->PopulateMap(ChildAttributedFoo::TypeIdClass(), ChildAttributedFoo::FillSignatures());	// populate the type reg instance
	//}
	//else
	//{
	//	throw std::logic_error("TypeRegistry instance not initialized yet!");	// we havent initialized our register singleton yet
	//}
}

std::vector<Signature> ChildAttributedFoo::Signatures()
{
	return std::vector<Signature> {
		{ "health"s, Datum::DatumType::Int, 1, offsetof(ChildAttributedFoo, health) },
		{ "mana"s, Datum::DatumType::Float, 1, offsetof(ChildAttributedFoo, mana) },
		{ "name"s, Datum::DatumType::String, 1, offsetof(ChildAttributedFoo, name) },
		{ "forwardVector"s, Datum::DatumType::Vec4, 1, offsetof(ChildAttributedFoo, forwardVector) },
		{ "theMatrix"s, Datum::DatumType::Mat4x4, 1, offsetof(ChildAttributedFoo, theMatrix) },
		{ "levelsArray"s, Datum::DatumType::Int, ChildAttributedFoo::arraySize, offsetof(ChildAttributedFoo, levelsArray)},
	};
}

std::vector<Signature> ChildAttributedFoo::FillSignatures()
{
	std::vector<Signature> parentAttributes = AttributedFoo::Signatures();
	std::vector<Signature> myAttributes = Signatures();

	//for (auto sig : myAttributes)	// for every signature in my attributes, place them into my parent's attributes
	//{
	//	parentAttributes.push_back(sig);
	//}

	//return parentAttributes;

	for (auto sig : parentAttributes)	// for every signature in my parent's attributes, place them into my attributes
	{
		myAttributes.push_back(sig);
	}

	return myAttributes;
}
