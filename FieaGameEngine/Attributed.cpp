#include "pch.h"
#include "Attributed.h"

using namespace Fiea::GameEngine;
RTTI_DEFINITIONS(Attributed);

/// @brief Constructor
/// @param id 
Attributed::Attributed(size_t id)
{
	mTypeId = id;
	_sigs = TypeRegistry::instance->GetSignatures(id);	// set Attributed._sigs to the signatures of this child class
	PopulateScopePrescribed();	// populate prescribed attributes of this child class with these signatures
}

/// @brief copy constructor
/// @param rhs 
Attributed::Attributed(const Attributed& rhs) : Scope(rhs), _sigs(TypeRegistry::instance->GetSignatures(rhs.mTypeId))	// this calls the parent's copy constructor
{
	// after getting and settings the _sigs populate the presribed attributes
	PopulateScopePrescribed();
}

/// @brief Assignment operator
/// @param rhs 
/// @return Attributed&
Attributed& Attributed::operator=(const Attributed& rhs)
{
	//_sigs = TypeRegistry::instance->GetSignatures(rhs.mTypeId);
	_sigs = rhs._sigs;	// this doesnt refer to the physcal fields of the target object, it refers to rhs
	mTypeId = rhs.mTypeId;

	return *this;
}

/// @brief populate this scope with datums for each prescribed attribute
void Attributed::PopulateScopePrescribed()
{
	char* self = reinterpret_cast<char*>(this);	// get our address to calculate the addresses of our member vars
	int* iPtr = nullptr;	// create a bunch of empty pointers to use in our for loop
	float* fPtr = nullptr;
	string* sPtr = nullptr;
	glm::vec4* vPtr = nullptr;
	glm::mat4* mPtr = nullptr;
	RTTI* pPtr = nullptr;

	for (size_t i = 0; i < _sigs.size(); ++i)
	{
		switch (_sigs[i].type)	// switch behavior depending on the signature's type MOVE SETSTORAGE CALL HERE TO DATUM
		{
		case Datum::DatumType::Int:
			iPtr = reinterpret_cast<int*>(self + _sigs[i].offset);
			Append(_sigs[i].name).SetStorage(iPtr, _sigs[i].size, _sigs[i].size);
			break;
		case Datum::DatumType::Float:
			fPtr = reinterpret_cast<float*>(self + _sigs[i].offset);
			Append(_sigs[i].name).SetStorage(fPtr, _sigs[i].size, _sigs[i].size);
			break;
		case Datum::DatumType::String:
			sPtr = reinterpret_cast<string*>(self + _sigs[i].offset);
			Append(_sigs[i].name).SetStorage(sPtr, _sigs[i].size, _sigs[i].size);
			break;
		case Datum::DatumType::Vec4:
			vPtr = reinterpret_cast<glm::vec4*>(self + _sigs[i].offset);
			Append(_sigs[i].name).SetStorage(vPtr, _sigs[i].size, _sigs[i].size);
			break;
		case Datum::DatumType::Mat4x4:
			mPtr = reinterpret_cast<glm::mat4*>(self + _sigs[i].offset);
			Append(_sigs[i].name).SetStorage(mPtr, _sigs[i].size, _sigs[i].size);
			break;
		//case Datum::DatumType::Pointer:
		//break;
		default:
			break;
		}
	}
}

/// @brief Given a string, return a bool indicating whether it is the name of an attribute for this object.
/// @param Name 
/// @return bool
bool Attributed::IsAttribute(const std::string Name)
{
	//return (if(Find(Name) != nullptr));
	if (Find(Name) != nullptr)	// if Find() finds a Datum with passed in name
	{
		// then passed in name is an attribute of this class
		return true;
	}

	return false;
}

/// @brief Given a string, return a bool indicating whether it is the name of a prescribed attribute for this class.
/// @param Name 
/// @return bool
bool Attributed::IsPrescribedAttribute(const std::string Name)
{
	if (!_sigs.empty())
	{
		for (size_t i = 0; i < _sigs.size(); ++i)
		{
			if (_sigs[i].name == Name)	// we found the name passed in as a Prescribed Attribute in this class
			{
				return true;
			}
		}
	}
	return false;
}

/// @brief Given a string, return a bool indicating whether it is the name of an auxiliary attribute for this object.
/// @param Name 
/// @return bool
bool Attributed::IsAuxiliaryAttribute(const std::string Name)
{
	// If the name is a prescribed attribute then it cant be an aux attribute
	if (IsPrescribedAttribute(Name))
	{
		return false;
	}
	else if (IsAttribute(Name))	// if it's not a prescribed one but is an attribute, then it's gotta be auxiliary
	{
		return true;
	}

	return false;	// if it's neither than it's not an attribute at all
}

// 
// 

/// @brief Given a string (which must not be the name of a prescribed attribute), append a Datum to this Scope with the given name, and return a reference to that Datum.
///		   If the Datum already existed, return it. This should be a thin wrapper around Scope::Append, with only checks for accidentally appending a name which is a prescribed attribute.
/// @param Name 
/// @return Datum&
Datum& Attributed::AppendAuxiliaryAttribute(const std::string Name)
{
	if (IsAttribute(Name))
	{
		if (IsPrescribedAttribute(Name))	// If the Datum is prescribed then throw logic error
		{
			throw std::logic_error("Trying to append a Prescribed Attribute as Auxiliary!");
		}
		return Append(Name); // this will find the existing Auxilary attribute
	}
	else
	{
		return Append(Name); // this will create a new auxiliary attribute
	}
}