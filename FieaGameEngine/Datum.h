#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "FnewMacro.h"
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include "RTTI.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"


namespace Fiea::GameEngine
{
	class Scope;

	//using ToString = std::string(*)(const Datum& datum, size_t idx);
	//std::string IntToString(const Datum& datum, size_t idx) {return std::to_string(datum.Get<int>(idx))
	using std::string;
	using std::vector;
	// Datum stores an array of values. The values in a given Datum have a single type – either a primitive type or a user-defined type.
	// Each Datum object represents, in general, an array, and all of the elements in that array have the same type.
	// Different Datum objects, however, can store values of different types.
	// The unique thing about Datum is that it's type is defined at run time. It is a polymorphic homogenous container.
	// Once the type stored by a Datum is established, it cannot be changed
	class Datum
	{
		friend class Scope;
	public:
		Datum() // default constructor
		{
			// No type was given so the default will be "Unknown"
			_datType = DatumType::Unknown;
		}
		~Datum()	// destructor
		{
			if (externalMem == false)
			{
				// "new" approach
				switch (_datType)
				{
				case DatumType::Unknown:	// nothing to clear
					break;
				case DatumType::Int:
					delete[] intArr;
					break;
				case DatumType::Float:
					delete[] floatArr;
					break;
				case DatumType::String:
					delete[] stringArr;
					break;
				case DatumType::Vec4:
					delete[] vecArr;
					break;
				case DatumType::Mat4x4:
					delete[] matrixArr;
					break;
				case DatumType::Pointer:
					delete _contType.r;	// If i ever use new RTTI*[arrSize] then switch it to delete[]
					break;
				case DatumType::Table:
					delete[] _contType.t;
					break;
				default:
					break;
				}
			}
			else
			{
				// if this datum is EXTERNAL, then we NEVER NEW or DELETE memory, all we do is reset our pointers
				switch (_datType)
				{
				case DatumType::Unknown:	// nothing to clear
					break;
				case DatumType::Int:
					intArr = nullptr;
					break;
				case DatumType::Float:
					floatArr = nullptr;
					break;
				case DatumType::String:
					stringArr = nullptr;
					break;
				case DatumType::Vec4:
					vecArr = nullptr;
					break;
				case DatumType::Mat4x4:
					matrixArr = nullptr;
					break;
				case DatumType::Pointer:
					_contType.r = nullptr;
					break;
				case DatumType::Table:
					_contType.t = nullptr;
					break;
				default:
					break;
				}
			}
		}

		enum class DatumType
		{
			Unknown,
			Int,
			Float,
			String,
			Vec4,
			Mat4x4,
			Pointer,	// RTTI*
			Table	// Scope*
		} _datType = DatumType::Unknown;

		Datum(const Datum& rhs);	// copy constructor

		// Items of the stored type for a Datum may be added
		// Item values can by assigned either directly by a reference to a value OR a string representation of the value
		//void Add(int32_t value, size_t idx = 0);	// pass by value unsupported
		void Add(const int32_t& value, size_t idx = 0);	// pass by reference
		void SetIntFromString(string s, size_t idx = 0);	// sets an int in the array based off given string
		void Add(const float_t& value, size_t idx = 0);
		void SetFloatFromString(string s, size_t idx = 0);
		void Add(const string& value, size_t idx = 0);
		void Add(const glm::vec4& value, size_t idx = 0);
		void SetVectorFromString(string s, size_t idx = 0);
		void Add(const glm::mat4& value, size_t idx = 0);
		void SetMatrixFromString(string s, size_t idx = 0);
		void Add(Scope* val, size_t idx = 0);
		void Add(RTTI* val, size_t idx = 0);

		// Given an array and the number of elements in it,
		// assign the internal values array to the given array, and the size and capacity to the given number of elements.
		// Mark Datum as not owning this memory (e.g. store that in another member variable).
		void SetStorage(int32_t* arr, size_t size = 0, size_t cap = 0);
		void SetStorage(float_t* arr, size_t size = 0, size_t cap = 0);
		void SetStorage(string* arr, size_t size = 0, size_t cap = 0);
		void SetStorage(glm::vec4* arr, size_t size = 0, size_t cap = 0);
		void SetStorage(glm::mat4* arr, size_t size = 0, size_t cap = 0);
		void SetStorage(RTTI* arr, size_t size, size_t cap);

		// Items can be retrieved either directly by a reference to the stored value OR a string representation of the value
		template<class T>
		T& Get(size_t idx = 0);
		Scope* GetScope(size_t idx = 0);
		const Scope* GetScope(const size_t idx = 0) const;
		string GetIntToString(size_t idx = 0);	// converts arr[idx] int elem into a string and returns that
		string GetFloatToString(size_t idx = 0);
		string GetVectorToString(size_t idx = 0);
		string GetMatrixToString(size_t idx = 0);

		// ...or removed
		void Remove(size_t idx);
		// Size Manipulation
		template<class T>
		void SetSize(size_t nSize);
		//void SetSize1(size_t nSize);

		// Retrieve Info
		size_t Size() { return arrSize; }
		size_t Capacity() { return arrCap; }
		DatumType DatType() { return _datType; }

		// Operator Overrides
		bool operator==(const Datum& rhs) const;
		bool operator!=(const Datum& rhs) const;
		Datum& operator=(const Datum& rhs);
		Datum& operator=(Datum&& rhs);
		void operator=(int32_t i);	// scalar operator= overload
		void operator=(float_t f);
		void operator=(string s);
		void operator=(glm::vec4 v);
		void operator=(glm::mat4 m);
		void operator=(Scope* s);
		Scope& operator[](std::uint32_t index) { return *Get<Scope*>(index); };

		// Utility
		void Clear();


	private:
		union ContainerType
		{
			int32_t* i;
			float_t* f;
			string* s;
			glm::vec4* v;	// considered POD
			glm::mat4* m;	// considered POD
			RTTI** r;
			Scope** t;
			void* ptr;
		} _contType{};

		//vector<ContainerType> v;	// attempt to make Datum hold a vector but told it will get complicated
		// ContainerType* arr = nullptr;	// attempt to make a single array that can "polymorph" into _contType
		// instead I created pointers for each type that I can create arrays out of without having to initiliaze memory
		int32_t* intArr = nullptr;
		float_t* floatArr = nullptr;
		string* stringArr = nullptr;
		glm::vec4* vecArr = nullptr;
		glm::mat4* matrixArr = nullptr;
		//Scope** scopePtr = nullptr; // replaced with union Scope** t
		
		size_t arrSize = 0;
		size_t arrCap = 10;

		bool externalMem = false;
	};
}

#include "Datum.inl"