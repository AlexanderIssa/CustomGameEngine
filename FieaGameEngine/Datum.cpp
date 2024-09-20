#include "pch.h"
#include "Datum.h"

using namespace Fiea::GameEngine;

// ==================== ALLOCATION ====================
// Copy Constructor
Datum::Datum(const Datum& rhs)
{
	if (rhs.externalMem == false)
	{
		// check to see what type rhs Datum is of, then copy all values
		switch (rhs._datType)
		{
		case DatumType::Unknown:
			// error here
			break;
		case DatumType::Int:
			_datType = DatumType::Int;
			try
			{
				intArr = FNEW int32_t[rhs.arrSize];
				_contType.i = intArr;
			}
			catch (const std::bad_alloc&)	// modern day isValid check
			{
				throw std::bad_alloc();
			}
			for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
			{
				intArr[i] = rhs.intArr[i];
			}
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Float:
			_datType = DatumType::Float;
			try
			{
				floatArr = FNEW float_t[rhs.arrSize];
				_contType.f = floatArr;
			}
			catch (const std::bad_alloc&)	// modern day isValid check
			{
				throw std::bad_alloc();
			}
			for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
			{
				floatArr[i] = rhs.floatArr[i];
			}
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::String:
			_datType = DatumType::String;
			try
			{
				stringArr = FNEW string[rhs.arrSize];
				_contType.s = stringArr;
			}
			catch (const std::bad_alloc&)	// modern day isValid check
			{
				throw std::bad_alloc();
			}
			for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
			{
				stringArr[i] = rhs.stringArr[i];
			}
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Vec4:
			_datType = DatumType::Vec4;
			try
			{
				vecArr = FNEW glm::vec4[rhs.arrSize];
				_contType.v = vecArr;
			}
			catch (const std::bad_alloc&)	// modern day isValid check
			{
				throw std::bad_alloc();
			}
			for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
			{
				vecArr[i] = rhs.vecArr[i];
			}
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Mat4x4:
			_datType = DatumType::Mat4x4;
			try
			{
				matrixArr = FNEW glm::mat4[rhs.arrSize];
				_contType.m = matrixArr;
			}
			catch (const std::bad_alloc&)	// modern day isValid check
			{
				throw std::bad_alloc();
			}
			for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
			{
				matrixArr[i] = rhs.matrixArr[i];
			}
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Table:
			_datType = DatumType::Table;
			//_contType.t = rhs._contType.t;	// shallow copy
			try
			{
				_contType.t = FNEW Scope * [rhs.arrSize];
			}
			catch (const std::bad_alloc&)	// modern day isValid check
			{
				throw std::bad_alloc();
			}
			for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
			{
				_contType.t[i] = rhs._contType.t[i];
				//_contType.t[i] = new Scope(*rhs._contType.t[i]);
			}
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Pointer:
			_datType = DatumType::Pointer;
			try
			{
				_contType.r = rhs._contType.r;	// shallow copy
				//_contType.r = new RTTI* [rhs.arrSize];

				//for (size_t i = 0; i < rhs.arrSize; ++i)
				//{
				//	// Check if the source pointer is valid
				//	if (rhs._contType.r[i] != nullptr)
				//	{
				//		// Allocate memory for the destination pointer and copy the object
				//		_contType.r[i] = new RTTI(*rhs._contType.r[i]->Clone());
				//	}
				//	else
				//	{
				//		_contType.r[i] = nullptr;
				//	}
				//}
			}
			catch (const std::bad_alloc&)	// modern day isValid check
			{
				throw std::bad_alloc();
			}
			//for (int i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
			//{
			//	_contType.r[i] = rhs._contType.r[i];
			//}
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		default:
			break;
		}
	}
	else
	{
		// if this datum is EXTERNAL, then we NEVER NEW or DELETE memory, all we do is reset our pointers
		switch (rhs._datType)
		{
		case DatumType::Unknown:	// nothing to copy over here
			_datType = DatumType::Unknown;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Int:
			_datType = DatumType::Int;
			intArr = rhs.intArr;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Float:
			_datType = DatumType::Float;
			floatArr = rhs.floatArr;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::String:
			_datType = DatumType::String;
			stringArr = rhs.stringArr;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Vec4:
			_datType = DatumType::Vec4;
			vecArr = rhs.vecArr;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Mat4x4:
			_datType = DatumType::Mat4x4;
			matrixArr = rhs.matrixArr;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Pointer:
			_datType = DatumType::Pointer;
			_contType.r = rhs._contType.r;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		case DatumType::Table:
			_datType = DatumType::Table;
			_contType.t = rhs._contType.t;
			arrSize = rhs.arrSize;
			arrCap = rhs.arrCap;
			externalMem = rhs.externalMem;
			break;
		default:
			break;
		}
	}
	

}

// Scope
void Datum::Add(Scope* value, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Table;
		try
		{
			//_contType.t = new Scope*[arrSize];
			//*_contType.t = value;
			_contType.t = FNEW Scope*; // try this later: new Scope*[arrSize];
			_contType.t[0] = value;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Table)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			_contType.t[idx] = value;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			_contType.t[idx] = value;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a int to a datum that doesn't contain type int!");
}

// RTTI
void Datum::Add(RTTI* value, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Pointer;
		try
		{
			//_contType.t = new Scope*[arrSize];
			//*_contType.t = value;
			_contType.r = FNEW RTTI*; // try this later: new Scope*[arrSize];
			_contType.r[0] = value;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Pointer)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			_contType.r[idx] = value;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			_contType.r[idx] = value;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a int to a datum that doesn't contain type int!");
}

// Integers
void Datum::Add(const int32_t& value, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Int;
		try
		{
			intArr = new int32_t[arrCap];
			_contType.i = intArr;
			intArr[0] = value;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Int)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			intArr[idx] = value;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			intArr[idx] = value;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a int to a datum that doesn't contain type int!");
}
void Datum::SetIntFromString(string s, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Int;
		try
		{
			intArr = FNEW int32_t[arrCap];
			_contType.i = intArr;
			intArr[0] = stoi(s);
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Int)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			intArr[idx] = stoi(s);
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			intArr[idx] = stoi(s);
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a int to a datum that doesn't contain type int!");
}
void Datum::SetStorage(int32_t* arr, size_t size, size_t cap)
{
	if (_datType == DatumType::Int)	// if we already have storage then clear it
	{
		//Clear();
		_contType.i = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Int;

		intArr = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else
	{
		throw std::logic_error("Trying to set storage for the wrong type!");
	}
}

// Floats
void Datum::Add(const float_t& value, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Float;
		try
		{
			floatArr = FNEW float_t[arrCap];
			_contType.f = floatArr;
			floatArr[0] = value;
			arrSize++;	// increment size to be 1
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Float)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			floatArr[idx] = value;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			floatArr[idx] = value;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a float to a datum that doesn't contain type float!");
}
void Datum::SetFloatFromString(string s, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Float;
		try
		{
			floatArr = FNEW float_t[arrCap];
			_contType.f = floatArr;
			floatArr[0] = stof(s);
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Float)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			floatArr[idx] = stof(s);
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			floatArr[idx] = stof(s);
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a float to a datum that doesn't contain type float!");
}
void Datum::SetStorage(float_t* arr, size_t size, size_t cap)
{
	if (_datType == DatumType::Float)	// if we already have storage then clear it
	{
		//Clear();
		_contType.f = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Float;

		floatArr = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else
	{
		throw std::logic_error("Trying to set storage for the wrong type!");
	}
}

// Strings
void Datum::Add(const string& value, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::String;
		try
		{
			stringArr = FNEW string[arrCap];
			_contType.s = stringArr;
			stringArr[0] = value;
			arrSize++;	// increment size to be 1
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::String)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			stringArr[idx] = value;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			stringArr[idx] = value;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a string to a datum that doesn't contain type string!");
}
void Datum::SetStorage(string* arr, size_t size, size_t cap)
{
	if (_datType == DatumType::String)	// if we already have storage then clear it
	{
		//Clear();
		_contType.s = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::String;

		stringArr = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else
	{
		throw std::logic_error("Trying to set storage for the wrong type!");
	}
}

// 4D Vectors
void Datum::Add(const glm::vec4& value, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Vec4;
		try
		{
			vecArr = FNEW glm::vec4[arrCap];
			_contType.v = vecArr;
			vecArr[0] = value;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Vec4)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			vecArr[idx] = value;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			vecArr[idx] = value;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a vec4 to a datum that doesn't contain type vec4!");
}
void Datum::SetVectorFromString(string s, size_t idx)
{

	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Vec4;
		try
		{
			vecArr = FNEW glm::vec4[arrCap];
			_contType.v = vecArr;

			float f0, f1, f2, f3;
			int result = sscanf_s(s.c_str(), "(%f, %f, %f, %f)", &f0, &f1, &f2, &f3);
			if (result != 4) { throw std::invalid_argument("Invalid input!"); }
			glm::vec4 temp(f0, f1, f2, f3);

			vecArr[0] = temp;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Vec4)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			float f0, f1, f2, f3;
			int result = sscanf_s(s.c_str(), "(%f, %f, %f, %f)", &f0, &f1, &f2, &f3);
			if (result != 4) { throw std::invalid_argument("Invalid input!"); }
			glm::vec4 temp(f0, f1, f2, f3);

			vecArr[idx] = temp;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			float f0, f1, f2, f3;
			int result = sscanf_s(s.c_str(), "(%f, %f, %f, %f)", &f0, &f1, &f2, &f3);
			if (result != 4) { throw std::invalid_argument("Invalid input!"); }
			glm::vec4 temp(f0, f1, f2, f3);

			vecArr[idx] = temp;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a vec4 to a datum that doesn't contain type vec4!");
}
void Datum::SetStorage(glm::vec4* arr, size_t size, size_t cap)
{
	if (_datType == DatumType::Vec4)	// if we already have storage then clear it
	{
		//Clear();
		_contType.v = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Vec4;

		vecArr = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else
	{
		throw std::logic_error("Trying to set storage for the wrong type!");
	}
}

// 4D Matrices
void Datum::Add(const glm::mat4& value, size_t idx)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Mat4x4;
		try
		{
			matrixArr = FNEW glm::mat4[arrCap];
			_contType.m = matrixArr;
			matrixArr[0] = value;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Mat4x4)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			matrixArr[idx] = value;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			matrixArr[idx] = value;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}

	}
	throw std::logic_error("Trying to set a mat4 to a datum that doesn't contain type mat4!");
}
void Datum::SetMatrixFromString(string s, size_t idx)
{

	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Mat4x4;
		try
		{
			matrixArr = FNEW glm::mat4[arrCap];
			_contType.m = matrixArr;

			float f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15;
			int result = sscanf_s(s.c_str(), "[%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]", &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15);
			if (result != 16) { throw std::invalid_argument("Invalid input!"); }
			glm::mat4 temp(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);

			matrixArr[0] = temp;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Mat4x4)
	{
		if (idx >= arrCap)
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			float f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15;
			int result = sscanf_s(s.c_str(), "[%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]", &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15);
			if (result != 16) { throw std::invalid_argument("Invalid input!"); }
			glm::mat4 temp(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);

			matrixArr[idx] = temp;
			return;
		}
		else if (arrSize < arrCap)	// need to add another item and we can hold another one
		{
			float f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15;
			int result = sscanf_s(s.c_str(), "[%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]", &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15);
			if (result != 16) { throw std::invalid_argument("Invalid input!"); }
			glm::mat4 temp(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);

			matrixArr[idx] = temp;
			arrSize++;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
		if (idx < arrSize)
		{
			float f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15;
			int result = sscanf_s(s.c_str(), "[%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]", &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15);
			if (result != 16) { throw std::invalid_argument("Invalid input!"); }
			glm::mat4 temp(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);

			matrixArr[idx] = temp;
			return;
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to set a mat4 to a datum that doesn't contain type mat4!");
}
void Datum::SetStorage(glm::mat4* arr, size_t size, size_t cap)
{
	if (_datType == DatumType::Mat4x4)	// if we already have storage then clear it
	{
		//Clear();
		_contType.m = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Mat4x4;

		matrixArr = arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else
	{
		throw std::logic_error("Trying to set storage for the wrong type!");
	}
}

// RTTI
void Datum::SetStorage(RTTI* arr, size_t size, size_t cap)
{
	if (_datType == DatumType::Pointer)	// if we already have storage then clear it
	{
		//Clear();
		_contType.r = &arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Pointer;

		_contType.r = &arr;
		arrSize = size;
		arrCap = cap;
		externalMem = true;
	}
	else
	{
		throw std::logic_error("Trying to set storage for the wrong type!");
	}
}

// ==================== RETRIEVAL ====================
// Scope
Scope* Datum::GetScope(size_t idx)
{
	if (_datType == DatumType::Table)
	{
		if (idx < arrSize)
		{
			return _contType.t[idx];
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a Scope from a datum that doesn't contain type Scope!");
}

const Scope* Datum::GetScope(size_t idx) const
{
	if (_datType == DatumType::Table)
	{
		if (idx < arrSize)
		{
			return _contType.t[idx];
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a Scope from a datum that doesn't contain type Scope!");
}

// Integers
template<>
int32_t& Datum::Get<int32_t>(size_t idx)
{
	if (_datType == DatumType::Int)
	{
		if (idx < arrSize)
		{
			return intArr[idx];
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a int from a datum that doesn't contain type int!");
}
string Datum::GetIntToString(size_t idx)
{
	if (_datType == DatumType::Int)
	{
		if (idx < arrSize)
		{
			return std::to_string(intArr[idx]);
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a int from a datum that doesn't contain type int!");
}
// Floats
template<>
float_t& Datum::Get<float_t>(size_t idx)
{
	if (_datType == DatumType::Float)
	{
		if (idx < arrSize)
		{
			return floatArr[idx];
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a float from a datum that doesn't contain type float!");
}
string Datum::GetFloatToString(size_t idx)
{
	if (_datType == DatumType::Float)
	{
		if (idx < arrSize)
		{
			return std::to_string(floatArr[idx]);
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a float from a datum that doesn't contain type float!");
}

// Strings
template<>
string& Datum::Get<string>(size_t idx)
{
	if (_datType == DatumType::String)
	{
		if (idx < arrSize)
		{
			return stringArr[idx];
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a string from a datum that doesn't contain type string!");
}

// 4D Vectors
template<>
glm::vec4& Datum::Get<glm::vec4>(size_t idx)
{
	if (_datType == DatumType::Vec4)
	{
		if (idx < arrSize)
		{
			return vecArr[idx];
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a vec4 from a datum that doesn't contain type vec4!");
}
string Datum::GetVectorToString(size_t idx)
{
	if (_datType == DatumType::Vec4)
	{
		if (idx < arrSize)
		{
			return glm::to_string(vecArr[idx]);
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a vec4 from a datum that doesn't contain type vec4!");
}

// 4D Matrices
template<>
glm::mat4& Datum::Get<glm::mat4>(size_t idx)
{
	if (_datType == DatumType::Mat4x4)
	{
		if (idx < arrSize)
		{
			return matrixArr[idx];
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a mat4 from a datum that doesn't contain type mat4!");
}
string Datum::GetMatrixToString(size_t idx)
{
	if (_datType == DatumType::Mat4x4)
	{
		if (idx < arrSize)
		{
			return glm::to_string(matrixArr[idx]);
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	throw std::logic_error("Trying to get a mat4 from a datum that doesn't contain type mat4!");
}

// ==================== UTILITY ====================
void Datum::Clear()
{
	if (externalMem == false)
	{
		switch (_datType)
		{
		case DatumType::Unknown:
			// error here
			break;
		case DatumType::Int:
			delete[] intArr;
			arrSize = 0;
			arrCap = 0;
			break;
		case DatumType::Float:
			delete[] floatArr;
			arrSize = 0;
			arrCap = 0;
			break;
		case DatumType::String:
			//for (size_t i = 0; i < arrSize; ++i)
			//{
			//	stringArr[i].~basic_string();	// no need to do this, it auto destructs
			//}
			delete[] stringArr;
			arrSize = 0;
			arrCap = 0;
			break;
		case DatumType::Vec4:
			for (size_t i = 0; i < arrSize; ++i)
			{
				vecArr[i].glm::vec4::~vec4();
			}
			delete[] vecArr;
			arrSize = 0;
			arrCap = 0;
			break;
		case DatumType::Mat4x4:
			for (size_t i = 0; i < arrSize; ++i)
			{
				matrixArr[i].glm::mat4::~mat4();
			}
			delete[] matrixArr;
			arrSize = 0;
			arrCap = 0;
			break;
		case DatumType::Table:	// should a Datum deconstruct a scope?? probably not.
			delete[] _contType.t;
			arrSize = 0;
			arrCap = 0;
			break;
		case DatumType::Pointer:
			delete _contType.r;
			arrSize = 0;
			arrCap = 0;
			break;
		default:
			break;
		}
	}
	else
	{
		// if this datum is EXTERNAL, then we NEVER NEW or DELETE memory, all we do is reset our pointers
		intArr = nullptr;
		floatArr = nullptr;
		stringArr = nullptr;
		vecArr = nullptr;
		matrixArr = nullptr;
		switch (_datType)
		{
		case DatumType::Pointer:
			_contType.r = nullptr;
			break;
		case DatumType::Table:
			_contType.t = nullptr;
			break;
		default:
			break;
		}
		arrSize = 0;	// reset our "array" size
	}
}

bool Datum::operator==(const Datum& rhs) const
{
	if (arrSize != rhs.arrSize)	// size is different, can't be the same
	{
		return false;
	}
	if (_datType != rhs._datType)	// cant be the same, they hold different types
	{
		return false;
	}

	switch (_datType)
	{
	case DatumType::Unknown:
		// error here
		break;
	case DatumType::Int:
		for (size_t i = 0; i < rhs.arrSize; ++i)
		{
			if (intArr[i] != rhs.intArr[i])	// if the values in this index arent the same then return false
			{
				return false;
			}
		}
		break;
	case DatumType::Float:
		for (size_t i = 0; i < rhs.arrSize; ++i)
		{
			if (floatArr[i] != rhs.floatArr[i])	// if the values in this index arent the same then return false
			{
				return false;
			}
		}
		break;
	case DatumType::String:
		for (size_t i = 0; i < rhs.arrSize; ++i)
		{
			if (stringArr[i] != rhs.stringArr[i])	// if the values in this index arent the same then return false
			{
				return false;
			}
		}
		break;
	case DatumType::Vec4:
		for (size_t i = 0; i < rhs.arrSize; ++i)
		{
			if (vecArr[i] != rhs.vecArr[i])	// if the values in this index arent the same then return false
			{
				return false;
			}
		}
		break;
	case DatumType::Mat4x4:
		for (size_t i = 0; i < rhs.arrSize; ++i)
		{
			if (matrixArr[i] != rhs.matrixArr[i])	// if the values in this index arent the same then return false
			{
				return false;
			}
		}
		break;
	case DatumType::Table:
		for (size_t i = 0; i < rhs.arrSize; ++i)
		{
			if (_contType.t[i] != rhs._contType.t[i])
			{
				return false;
			}
		}
	case DatumType::Pointer:
		for (size_t i = 0; i < rhs.arrSize; ++i)
		{
			if (_contType.r[i] != rhs._contType.r[i])
			{
				return false;
			}
		}
	default:
		// error here?
		return false;
		break;
	}

	return true;
}

bool Datum::operator!=(const Datum& rhs) const
{
	return!(operator==(rhs));
}

Datum& Datum::operator=(const Datum& rhs)
{
	if (externalMem == rhs.externalMem)
	{
		if (rhs.externalMem == false)
		{
			if (_datType == DatumType::Unknown)
			{
				// this needs to account for a Datum that is type Unknown and assign it to rhs._datType
				switch (rhs._datType)
				{
				case DatumType::Unknown:
					// error here
					break;
				case DatumType::Int:
					_datType = DatumType::Int;
					try
					{
						intArr = FNEW int32_t[rhs.arrSize];
						_contType.i = intArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						intArr[i] = rhs.intArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Float:
					_datType = DatumType::Float;
					try
					{
						floatArr = FNEW float_t[rhs.arrSize];
						_contType.f = floatArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						floatArr[i] = rhs.floatArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::String:
					_datType = DatumType::String;
					try
					{
						stringArr = FNEW string[rhs.arrSize];
						_contType.s = stringArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						stringArr[i] = rhs.stringArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Vec4:
					_datType = DatumType::Vec4;
					try
					{
						vecArr = FNEW glm::vec4[rhs.arrSize];
						_contType.v = vecArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						vecArr[i] = rhs.vecArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Mat4x4:
					_datType = DatumType::Mat4x4;
					try
					{
						matrixArr = FNEW glm::mat4[rhs.arrSize];
						_contType.m = matrixArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						matrixArr[i] = rhs.matrixArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Table:
					_datType = DatumType::Table;
					try
					{
						_contType.t = FNEW Scope * [rhs.arrSize];
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						_contType.t[i] = rhs._contType.t[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Pointer:
					_datType = DatumType::Pointer;
					try
					{
						_contType.r = FNEW RTTI * [rhs.arrSize];
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						_contType.r[i] = rhs._contType.r[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				default:
					break;
				}
				return *this;
			}
			else if(_datType == rhs._datType)	// if _datType != Unknown
			{
				Clear();	// clear current array correctly then DESTROY every array

				// check to see what type rhs Datum is of, then copy all values (deep copy)
				switch (rhs._datType)
				{
				case DatumType::Unknown:
					// error here
					break;
				case DatumType::Int:
					_datType = DatumType::Int;
					try
					{
						intArr = FNEW int32_t[rhs.arrSize];
						_contType.i = intArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						intArr[i] = rhs.intArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Float:
					_datType = DatumType::Float;
					try
					{
						floatArr = FNEW float_t[rhs.arrSize];
						_contType.f = floatArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						floatArr[i] = rhs.floatArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::String:
					_datType = DatumType::String;
					try
					{
						stringArr = FNEW string[rhs.arrSize];
						_contType.s = stringArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						stringArr[i] = rhs.stringArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Vec4:
					_datType = DatumType::Vec4;
					try
					{
						vecArr = FNEW glm::vec4[rhs.arrSize];
						_contType.v = vecArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						vecArr[i] = rhs.vecArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				case DatumType::Mat4x4:
					_datType = DatumType::Mat4x4;
					try
					{
						matrixArr = FNEW glm::mat4[rhs.arrSize];
						_contType.m = matrixArr;
					}
					catch (const std::bad_alloc&)	// modern day isValid check
					{
						throw std::bad_alloc();
					}
					for (size_t i = 0; i < rhs.arrSize; ++i)	// copy all values (Deep copy / by value) into this new Datum
					{
						matrixArr[i] = rhs.matrixArr[i];
					}
					arrSize = rhs.arrSize;
					arrCap = rhs.arrCap;
					externalMem = rhs.externalMem;
					break;
				default:
					break;
				}
				return *this;
			}
			else // if _datType != rhs._datType
			{
				throw std::logic_error("trying to assign two Datums with different types!");
			}
		}
		else // if rhs.externalMem = true
		{
			// Don't destruct the elements inside our array, just reset our array so we can new more later
			intArr = nullptr;
			floatArr = nullptr;
			stringArr = nullptr;
			vecArr = nullptr;
			matrixArr = nullptr;

			switch (_datType)
			{
			case DatumType::Pointer:
				_contType.r = nullptr;
				//delete _contType.r;
				break;
			case DatumType::Table:
				_contType.t = nullptr;
				//delete[] _contType.t;
				break;
			default:
				break;
			}
			return *this;
		}
	}
	else // if externalMem != rhs.externalMem
	{
		throw std::logic_error("One of these Datums are internal and the other is external!");
	}
}

Datum& Datum::operator=(Datum&& rhs)
{
	_datType = std::move(rhs._datType);
	arrSize = std::move(rhs.arrSize);
	arrCap = std::move(rhs.arrCap);
	_contType = std::move(rhs._contType);
	intArr = std::move(rhs.intArr);
	floatArr = std::move(rhs.floatArr);
	stringArr = std::move(rhs.stringArr);
	externalMem = std::move(rhs.externalMem);

	rhs._datType = DatumType::Unknown;
	rhs.arrSize = 0;
	rhs.arrCap = 0;
	rhs._contType = {};
	//delete[] rhs.intArr;
	//delete[] rhs.floatArr;
	//delete[] rhs.stringArr;
	rhs.externalMem = false;

	return *this;
}

void Datum::operator=(int32_t i)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Int;
		try
		{
			intArr = FNEW int32_t[arrCap];
			_contType.i = intArr;
			intArr[0] = i;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Int)
	{
		if (intArr != nullptr)
		{
			intArr[0] = i;
			return;
		}
		else
		{
			throw std::runtime_error("Array not initialized!");
		}
	}
	throw std::logic_error("Trying to set a int to a datum that doesn't contain type int!");
}
void Datum::operator=(float_t f)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Float;
		try
		{
			floatArr = FNEW float_t[arrCap];
			_contType.f = floatArr;
			floatArr[0] = f;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Float)
	{
		if (floatArr != nullptr)
		{
			floatArr[0] = f;
			return;
		}
		else
		{
			throw std::runtime_error("Array not initialized!");
		}
	}
	throw std::logic_error("Trying to set a float to a datum that doesn't contain type float!");
}
void Datum::operator=(string s)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::String;
		try
		{
			stringArr = FNEW string[arrCap];
			_contType.s = stringArr;
			stringArr[0] = s;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::String)
	{
		if (stringArr != nullptr)
		{
			stringArr[0] = s;
			return;
		}
		else
		{
			throw std::runtime_error("Array not initialized!");
		}
	}
	throw std::logic_error("Trying to set a string to a datum that doesn't contain type string!");
}
void Datum::operator=(glm::vec4 v)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Vec4;
		try
		{
			vecArr = FNEW glm::vec4[arrCap];
			_contType.v = vecArr;
			vecArr[0] = v;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Vec4)
	{
		if (vecArr != nullptr)
		{
			vecArr[0] = v;
			return;
		}
		else
		{
			throw std::runtime_error("Array not initialized!");
		}
	}
	throw std::logic_error("Trying to set a vec4 to a datum that doesn't contain type vec4!");
}
void Datum::operator=(glm::mat4 m)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Mat4x4;
		try
		{
			matrixArr = FNEW glm::mat4[arrCap];
			_contType.m = matrixArr;
			matrixArr[0] = m;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Mat4x4)
	{
		if (matrixArr != nullptr)
		{
			matrixArr[0] = m;
			return;
		}
		else
		{
			throw std::runtime_error("Array not initialized!");
		}
	}
	throw std::logic_error("Trying to set a mat4 to a datum that doesn't contain type mat4!");
}
void Datum::operator=(Scope* s)
{
	if (_datType == DatumType::Unknown)
	{
		_datType = DatumType::Table;
		try
		{
			_contType.t = FNEW Scope*[arrCap];
			_contType.t[0] = s;
			arrSize++;
			return;
		}
		catch (const std::bad_alloc&)	// modern day isValid check
		{
			throw std::bad_alloc();
		}
	}
	if (_datType == DatumType::Table)
	{
		if (_contType.t != nullptr)
		{
			_contType.t[0] = s;
			return;
		}
		else
		{
			throw std::runtime_error("Array not initialized!");
		}
	}
	throw std::logic_error("Trying to set a int to a datum that doesn't contain type int!");
}

template<>
void Datum::SetSize<int32_t>(size_t nSize)
{
	if (externalMem)
	{
		throw std::logic_error("Cannot resize external memory!");
	}
	if (_datType != DatumType::Unknown)
	{
		if (arrCap >= nSize)	// we have enough capacity to accomedate the new size
		{
			arrSize = nSize;
		}
		if (_datType == DatumType::Int)	// if the Datum hold the correct type
		{
			if (nSize > arrSize)
			{
				int32_t* iNewMem = FNEW int32_t[nSize];
				assert(iNewMem != nullptr);

				for (size_t i = 0; i < arrSize; ++i)
				{
					iNewMem[i] = intArr[i];
					//new(iNewMem + i) int32_t(intArr[i]);		// placement new
				}
				for (size_t i = 0; i < arrSize; ++i)
				{
					intArr[i] = 0;	// this is pretty useless but it's nice to keep consistent
				}
				delete[] intArr;
				intArr = iNewMem;
				arrSize = nSize;
				arrCap = nSize;
			}
			else
			{
				// lower the size which means destroying anything past nSize element
			}
		}
		else
		{
			throw std::logic_error("trying to resize a Datum of the wrong type!");
		}
	}
}
template<>
void Datum::SetSize<float_t>(size_t nSize)
{
	if (externalMem)
	{
		throw std::logic_error("Cannot resize external memory!");
	}
	if (_datType != DatumType::Unknown)
	{
		if (arrCap >= nSize)	// we have enough capacity to accomedate the new size
		{
			arrSize = nSize;
		}
		if (_datType == DatumType::Float)	// if the Datum hold the correct type
		{
			if (nSize > arrSize)
			{
				float_t* fNewMem = FNEW float_t[nSize];
				assert(fNewMem != nullptr);

				for (size_t i = 0; i < arrSize; ++i)
				{
					fNewMem[i] = floatArr[i];
					//new(fNewMem + i) float_t(floatArr[i]);	// placement new
				}
				for (size_t i = 0; i < arrSize; ++i)
				{
					floatArr[i] = 0;	// this is pretty useless but it's nice to keep consistent
				}
				delete[] floatArr;
				floatArr = fNewMem;
				arrSize = nSize;
				arrCap = nSize;
			}
		}
		else
		{
			throw std::logic_error("trying to resize a Datum of the wrong type!");
		}
	}
}
template<>
void Datum::SetSize<string>(size_t nSize)
{
	if (externalMem)
	{
		throw std::logic_error("Cannot resize external memory!");
	}
	if (_datType != DatumType::Unknown)
	{
		if (arrCap >= nSize)	// we have enough capacity to accomedate the new size
		{
			arrSize = nSize;
		}
		if (_datType == DatumType::String)	// if the Datum hold the correct type
		{
			if (nSize > arrSize)
			{
				string* sNewMem = FNEW string[nSize];
				assert(sNewMem != nullptr);

				for (size_t i = 0; i < arrSize; ++i)
				{
					sNewMem[i] = stringArr[i];
					//new(sNewMem + i) string(stringArr[i]);	// placement new
				}
				for (size_t i = 0; i < arrSize; ++i)
				{
					stringArr[i].~basic_string();	// decon each element
				}
				delete[] stringArr;
				stringArr = sNewMem;
				arrSize = nSize;
				arrCap = nSize;
			}
		}
		else
		{
			throw std::logic_error("trying to resize a Datum of the wrong type!");
		}
	}
}
template<>
void Datum::SetSize<glm::vec4>(size_t nSize)
{
	if (externalMem)
	{
		throw std::logic_error("Cannot resize external memory!");
	}
	if (_datType != DatumType::Unknown)
	{
		if (arrCap >= nSize)	// we have enough capacity to accomedate the new size
		{
			arrSize = nSize;
		}
		if (_datType == DatumType::Vec4)	// if the Datum hold the correct type
		{
			if (nSize > arrSize)
			{
				glm::vec4* vNewMem = FNEW glm::vec4[nSize];
				assert(vNewMem != nullptr);

				for (size_t i = 0; i < arrSize; ++i)
				{
					vNewMem[i] = vecArr[i];
					//new(vNewMem + i) glm::vec4(vecArr[i]);	// placement new
				}
				for (size_t i = 0; i < arrSize; ++i)
				{
					vecArr[i].glm::vec4::~vec4();	// decon each element
				}
				delete[] vecArr;
				vecArr = vNewMem;
				arrSize = nSize;
				arrCap = nSize;
			}
		}
		else
		{
			throw std::logic_error("trying to resize a Datum of the wrong type!");
		}
	}
}
template<>
void Datum::SetSize<glm::mat4>(size_t nSize)
{
	if (externalMem)
	{
		throw std::logic_error("Cannot resize external memory!");
	}
	if (_datType != DatumType::Unknown)
	{
		if (arrCap >= nSize)	// we have enough capacity to accomedate the new size
		{
			arrSize = nSize;
		}
		if (_datType == DatumType::Mat4x4)	// if the Datum hold the correct type
		{
			if (nSize > arrSize)
			{
				glm::mat4* mNewMem = FNEW glm::mat4[nSize];
				assert(mNewMem != nullptr);

				for (size_t i = 0; i < arrSize; ++i)
				{
					mNewMem[i] = matrixArr[i];
					//new(mNewMem + i) glm::mat4(matrixArr[i]);	// placement new
				}
				for (size_t i = 0; i < arrSize; ++i)
				{
					matrixArr[i].glm::mat4::~mat4();	// decon each element
				}

				delete[] matrixArr;
				matrixArr = mNewMem;
				arrSize = nSize;
				arrCap = nSize;
			}
		}
		else
		{
			throw std::logic_error("trying to resize a Datum of the wrong type!");
		}
	}
}

void Datum::Remove(size_t idx)
{
	if (_datType != DatumType::Unknown && arrSize > 0)
	{
		if (idx < arrSize)	// the index exists
		{
			switch (_datType)
			{
			case DatumType::Unknown:
				// error here
				break;
			case DatumType::Int:
				if (idx < arrSize - 1)
				{
					for (size_t i = idx; i < arrSize - 1; ++i)
					{
						// replace the element that we are targetting with it's next one
						// and then move everything in front of it down one and remove the furthest element
						intArr[i] = intArr[i + 1];
					}
					intArr[arrSize - 1] = 0;	// deconstruct the last element
					arrSize--;
				}
				return;
				break;
			case DatumType::Float:
				if (idx < arrSize - 1)
				{
					for (size_t i = idx; i < arrSize - 1; ++i)
					{
						// replace the element that we are targetting with it's next one
						// and then move everything in front of it down one and remove the furthest element
						floatArr[i] = floatArr[i + 1];
					}
					floatArr[arrSize - 1] = 0;	// deconstruct the last element
					arrSize--;
				}
				return;
				break;
			case DatumType::String:
				if (idx < arrSize - 1)
				{
					for (size_t i = idx; i < arrSize - 1; ++i)
					{
						// replace the element that we are targetting with it's next one
						// and then move everything in front of it down one and remove the furthest element
						stringArr[i] = stringArr[i + 1];
					}
					stringArr[arrSize - 1].~basic_string();	// deconstruct the last element
					arrSize--;
				}
				return;
				break;
			case DatumType::Vec4:
				if (idx < arrSize - 1)
				{
					for (size_t i = idx; i < arrSize - 1; ++i)
					{
						// replace the element that we are targetting with it's next one
						// and then move everything in front of it down one and remove the furthest element
						vecArr[i] = vecArr[i + 1];
					}
					vecArr[arrSize - 1].glm::vec4::~vec4();	// deconstruct the last element
					arrSize--;
				}
				return;
				break;
			case DatumType::Mat4x4:
				if (idx < arrSize - 1)
				{
					for (size_t i = idx; i < arrSize - 1; ++i)
					{
						// replace the element that we are targetting with it's next one
						// and then move everything in front of it down one and remove the furthest element
						matrixArr[i] = matrixArr[i + 1];
					}
					matrixArr[arrSize - 1].glm::mat4::~mat4();	// deconstruct the last element
					arrSize--;
				}
				return;
				break;
			default:
				break;
			}
		}
		else
		{
			throw std::out_of_range("Idx out of range!");
		}
	}
	else
	{
		throw std::logic_error("Trying to remove something from a Datum that hasn't been initialized or has 0 size");
	}
}