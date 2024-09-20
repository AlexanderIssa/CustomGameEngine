#pragma once

#include "FnewMacro.h"
#include "Scope.h"
#include "Signature.h"
#include "TypeRegistry.h"


namespace Fiea::GameEngine
{
	// thin wrapper around scopes that wires cpp code to its base data
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);
	public:
		Attributed() = default;	// default constructor
		virtual bool IsAttribute(const std::string Name);	// Given a string, return a bool indicating whether it is the name of an attribute for this object.
		virtual bool IsPrescribedAttribute(const std::string Name);	// Given a string, return a bool indicating whether it is the name of a prescribed attribute for this class.
		virtual bool IsAuxiliaryAttribute(const std::string Name); // Given a string, return a bool indicating whether it is the name of an auxiliary attribute for this object.
		virtual Datum& AppendAuxiliaryAttribute(const std::string Name);	//Given a string (which must not be the name of a prescribed attribute), append a Datum to this Scope with the given name, and return a reference to that Datum.

	protected:
		Attributed(size_t id);
		virtual ~Attributed() = default;	// deconstructor
		Attributed(Attributed&& rhs) noexcept = default;	// move constructor
		Attributed(const Attributed& rhs);	// copy constructor
		virtual Attributed* Clone() const = 0;
		Attributed& operator=(const Attributed& rhs);// = default;
		//Attributed& operator=(const Attributed&& rhs) noexcept = default;


	private:
		void PopulateScopePrescribed();

		//RTTI* _ptr = nullptr;
		vector<Signature> _sigs;
		size_t mTypeId;
	};
}