#include "pch.h"
#include "AttributedFoo.h"

using namespace Fiea::GameEngine;
using namespace std::string_literals;
RTTI_DEFINITIONS(AttributedFoo);

// Default constructor
AttributedFoo::AttributedFoo() : Attributed(AttributedFoo::TypeIdClass())
{

}

AttributedFoo::AttributedFoo(size_t id) : Attributed(id)	// Child sent id to me, place it's attributes with mine into the type reg
{

}

// cloneable implementation
AttributedFoo* AttributedFoo::Clone() const
{
	return FNEW AttributedFoo(*this);
}

// Comparison operator override
bool AttributedFoo::operator==(const AttributedFoo& rhs) const
{
	if (externalInteger != rhs.externalInteger) return false;
	if (externalFloat != rhs.externalFloat) return false;
	if (externalVector != rhs.externalVector) return false;
	if (externalString != rhs.externalString) return false;
	if (externalMatrix != rhs.externalMatrix) return false;

	return true;
}

// Return the Prescribed attributes of this class
std::vector<Signature> AttributedFoo::Signatures()
{
	return std::vector<Signature> {
		{ "externalInteger"s, Datum::DatumType::Int, 1, offsetof(AttributedFoo, externalInteger) },
		{ "externalFloat"s, Datum::DatumType::Float, 1, offsetof(AttributedFoo, externalFloat) },
		{ "externalString"s, Datum::DatumType::String, 1, offsetof(AttributedFoo, externalString) },
		{ "externalVector"s, Datum::DatumType::Vec4, 1, offsetof(AttributedFoo, externalVector) },
		{ "externalMatrix"s, Datum::DatumType::Mat4x4, 1, offsetof(AttributedFoo, externalMatrix) },
		//{ "externalPointer"s, Datum::DatumType::Pointer, 1, offsetof(AttributedFoo, externalPointer) },
		{ "externalIntegerArray"s, Datum::DatumType::Int, arraySize, offsetof(AttributedFoo, externalIntegerArray) },
		{ "externalFloatArray"s, Datum::DatumType::Float, arraySize, offsetof(AttributedFoo, externalFloatArray) },
		{ "externalStringArray"s, Datum::DatumType::String, arraySize, offsetof(AttributedFoo, externalStringArray) },
		{ "externalVectorArray"s, Datum::DatumType::Vec4, arraySize, offsetof(AttributedFoo, externalVectorArray) },
		{ "externalMatrixArray"s, Datum::DatumType::Mat4x4, arraySize, offsetof(AttributedFoo, externalMatrixArray) },
		//{ "externalPointerArray"s, Datum::DatumType::Pointer, arraySize, offsetof(AttributedFoo, externalPointerArray) }
	};
}