#include "pch.h"
#include "Scope.h"

using namespace Fiea::GameEngine;
RTTI_DEFINITIONS(Scope);

/// @brief Default Constructor
/// @param cap 
Scope::Scope(size_t cap)
{
	_data.reserve(cap);
	_vData.reserve(cap);
}

/// @brief Copy Constructor
/// @param other 
Scope::Scope(const Scope& other)
{
	_parent = nullptr;	// make sure this Scope's parent is null
	
	_data = other._data;	// deep copy over the map

	_vData = other._vData;	// bad shallow copy!

	//for (int i = 0; i < other._vData.size(); ++i)	// deep copy vector of Datum*, make sure not to copy the pointers directly
	//{
	//	Datum temp1(*other._vData[i]);
	//	Datum* temp = &temp1;

	//	_vData[i] = temp;
	//	//_vData[i] = other._vData[i];

	//}

	for (const auto& pair : other._data)	// fill up our vector with pointers that point to our datums in our map and not RHS's map 
	{
		//Datum* temp = &Append(pair.first);
		//_vData.push_back(temp);
		const Datum* otherDatum = other.Find(pair.first);
		for (size_t i = 0; i < _vData.size(); ++i)
		{
			if (_vData[i] == otherDatum)
			{
				_vData[i] = Find(pair.first);
			}
		}
		//_vData.push_back(&pair.second);
	}

	// should be const auto pair&, need to make const correct functions in Datum
	for (auto pair : other._data)
	{
		if (pair.second._datType == Datum::DatumType::Table)	// there is a child scope that needs to be deep copied
		{
			for (size_t i = 0; i < pair.second.Size(); ++i)
			{
				Scope& s = AppendScope(pair.first);	
				s = *pair.second.GetScope(i);	// deep copy child scopes in this one key
			}
		}
		else
		{
			Datum& d = Append(pair.first);
			d = pair.second;	// deep copy datums in this one key
		}
	}
}

/// @brief Copy Constructor
/// @param other 
Scope::Scope(Scope&& other) noexcept
{

	_data = std::move(other._data);	// move the map from other to this scope
	_vData = std::move(other._vData);	// move the vector from other to this scope
	_vInternalScopes = std::move(other._vInternalScopes);
}

/// @brief Destructor
Scope::~Scope()
{
	if (_parent != nullptr)
	{
		Scope* temp = Orphan();	// cleanly orphan the parent scope _parent->Orphan();
	}
	//DestroyChildScopes();	// make sure this doesn't infinitely loop (spoiler: it does right now)
	//for (Scope* iScope : _vInternalScopes)
	//{
	//	delete iScope;	// delete child scope*s
	//}
	//_vInternalScopes.clear();
	//DestroyDatums();
	//_parent = nullptr;
	Clear();
}

// Operator Overrides

/// @brief Assignment Operator
/// @param rhs 
/// @return *this
Scope& Scope::operator=(const Scope& rhs)
{
	if (this != &rhs)	// if we are not copying ourselves
	{
		Scope* child = Orphan();	// get rid of this scope's parent and tell the parent it's gone, returns "this"

		Clear();

		// Copy storage element by element (deep copy)
		_data = rhs._data;	// deep copy over the map

		for (auto pair : _data)	// fill up our vector with pointers that point to our datums in our map and not RHS's map 
		{
			_vData.push_back(&pair.second);
		}

		for (auto pair : rhs._data)
		{
			if (pair.second._datType == Datum::DatumType::Table)	// there is a child scope that needs to be deep copied
			{
				for (size_t i = 0; i < pair.second.Size(); ++i)
				{
					Scope& s = AppendScope(pair.first);
					s = *pair.second.GetScope(i);	// deep copy child scopes in this one key
				}
			}
			else
			{
				Datum& d = Append(pair.first);
				d = pair.second;	// deep copy datums in this one key
			}
		}
	}

	return *this;
}

/// @brief Brackets operator, wraps Append for syntatic convenience
/// @param key 
/// @return Datum&
Datum& Scope::operator[](const string key)
{
	return Append(key);
}

/// @brief takes an unsigned integer return a reference to a Datum at the given index. 
///			The index values correspond to the order in which items were appended.
/// @param idx 
/// @return Datum&
Datum& Scope::operator[](size_t idx)
{
	return *_vData[idx];
}

/// @brief const version of above
/// @param idx 
/// @return const Datum&
const Datum& Scope::operator[](size_t idx) const
{
	return *_vData[idx];
}

Scope* Scope::Clone() const
{
	return FNEW Scope(*this);
}

/// @brief Comparison operator
/// @param rhs 
/// @return bool
bool Scope::operator==(const Scope& rhs) const
{
	if (_vData.size() != rhs._vData.size())	// size is different, can't be the same
	{
		return false;
	}
	if (_data.size() != rhs._data.size())	// cant be the same, they hold different types
	{
		return false;
	}
	// Ok now let's actually check the contents
	for (size_t i = 0; i < _vData.size(); ++i)	// start with the vector of Datum*
	{
		if (_vData[i] != rhs._vData[i])
		{
			return false;	// elements don't match
		}
	}
	if (_parent != nullptr && rhs._parent == nullptr)
	{
		return false;
	}
	else if (_parent == nullptr && rhs._parent != nullptr)
	{
		return false;
	}
	return true;
}

bool Scope::operator!=(const Scope& rhs) const
{
	return !operator==(rhs);
}

///// @brief Destruct child scopes
//void Scope::DestroyChildScopes()
//{
//	for (Datum* _datum : _vData)
//	{
//		if (_datum->_datType == Datum::DatumType::Table)
//		{
//			//delete(_datum->_contType.t);
//			for (size_t i = 0; i < _datum->Size(); ++i)
//			{
//				Scope* s = _datum->GetScope(i);
//				s->~Scope();
//			}
//			//*_datum->Clear();
//			//*_datum->_contType.t->Clear();
//			//delete _datum;
//		}
//	}
//}
//
///// @brief Destroys all datums contained in this scope
//void Scope::DestroyDatums()
//{
//	for (Datum* _datum : _vData)
//	{
//		_datum->~Datum();
//	}
//
//	_vData.clear();
//}

/// @brief  Remove a Datum containing a child scope from our vector, disowning that child scope from this parent scope
/// @param childDatum 
void Scope::DisownChild(Datum* childDatum, std::string childName)
{
	auto it = std::remove(_vData.begin(), _vData.end(), childDatum);
	_vData.erase(it, _vData.end());

	if (childName != "")
	{
		auto it2 = _data.find(childName);
		if (it2 != _data.end()) {	// delete the datum from the map as well
			_data.erase(it2);
		}
	}
}

// Only get's rid of this scope's parent, nothing else
// Called from the child being Orphaned.
// Look through parent's vector of datum's to see if child can find itself in one of the Datums
// If it can then remove that datum from the parent scope's vector of datum* and remove it from the parent's map of datums
Scope* Scope::Orphan()
{
	if (_parent != nullptr)
	{
		Datum* temp = nullptr;
		for (Datum* datum : _parent->_vData)
		{
			if (datum->_datType == Datum::DatumType::Table)	// only try to remove a child scope if the Datum holds scopes
			{
				if (datum->GetScope() == this)
				{
					temp = datum;
					break;	// no need to continue loop, we found ourselves
				}
			}
		}

		if (temp != nullptr && _parent != nullptr)	// if we found ourselves in our parent's vector of Datums then remove that Datum, that contains us, out of our parent's vector
		{
			_parent->DisownChild(temp);
		}
		_parent = nullptr;	// get rid of parent of this child scope
	}
	
	return this;	// return the orphaned child scope (this) so we don't lose it
}

/// @brief Appends a new Datum into the map using the passed in string as the key
/// @param key 
/// @return Datum&
Datum& Scope::Append(const std::string& key)
{
	if (auto result = _data.find(key); result != _data.end())
	{
		return result->second;	// you found it!
	}
	Datum temp;
	_data[key] = temp;
	_vData.push_back(&_data[key]);
	return _data[key];  // map operator[] will retrieve the value at that key, but if there are none it will default const a Datum with that key
}

// return a reference to a Scope with the associated name.
// If a Datum already exists at that key reuse it (and append to it a new Scope), otherwise create a new Datum.
Scope& Scope::AppendScope(const std::string& key)
{
	Datum& d = Append(key);
	if (d._datType != Datum::DatumType::Table && d._datType != Datum::DatumType::Unknown)
	{
		throw std::logic_error("Trying to Append a scope to a Datum that can't hold a Scope");
	}
	Scope* s = FNEW Scope;	// PUT THIS INTERNAL CHILD SCOPE SOMEWHERE IN THIS SCOPE TO DELETE IT LATER
	_vInternalScopes.push_back(s);
	s->_parent = this;	// set this child scope's parent to this scope
	d.Add(s);	// insert this scope into Datum's table and set the DatumType to Table (scope)

	return *d.GetScope();	// return the address of this scope that should be in the Datum's table (but what if there's multiple scopes?)
}

// takes a reference to a Scope (the child to adopt), a string (the name of key for the Datum to use for storing the child)
// but Scope may only have 1 parent... so if a scope is adopted, it must be removed from it's prior parent.
// Scope& child might want to be const but I'm not sure how I would keep it const but change it's parent
// Called from the parent that will adopt passed child
void Scope::Adopt(Scope& child, const string& childName, Scope* parentOverride)
{
	Scope* childPtr = child.Orphan(); //DisownChild(child); // get rid of the adopted child's parent 

	if (parentOverride != nullptr)
	{
		child._parent = parentOverride;	// in the case where we dont want this scope to be this child's parent
	}
	else
	{
		child._parent = this;	// set it's new parent to this scope
	}
	Datum* d = &Append(childName); // create a new datum with child's name in this scope's map
	d->Add(&child);	// or maybe childPtr // insert child scope into this new Datum that has the child's name as it's key in the parent scope's (this) map
	// the issue with this "Add(&child)" is that without an idx param sent it'll just replace the first element, aka a Datum can only hold 1 scope at a time (bad)
	// I need a push_back() for datum...

	//Scope* temp = child.Orphan(); // get rid of the adopted child's parent 
	//child.Reparent(*this);	//and set it's new parent to this scope
	//Datum datum = Append(childName);	// create a new datum with child's name in this scope's map
	//datum.Add(&child);	// insert child scope into this new Datum that has the child's name as it's key in the map
}

/// @brief Adopt overload for RHS scopes
/// @param child 
/// @param childName 
void Scope::Adopt(Scope*&& child, const string& childName, Scope* parentOverride)
{
	Scope* childPtr = std::move(child);	// Turn child back into an RValue and make it an LValue called childPtr
	Scope* temp = childPtr->Orphan();	// orphan it
	if (parentOverride != nullptr)
	{
		childPtr->_parent = parentOverride;	// in the case where we dont want this scope to be this child's parent
	}
	else
	{
		childPtr->_parent = this;	// set it's new parent to this scope
	}

	Datum* d = &Append(childName);	// make a datum that points to the pointer
	d->Add(childPtr);

	_vInternalScopes.push_back(childPtr);	// this scope is responsible for deleting the memory of the child passed in
}

// Takes a constant string and returns the address of a Datum.
// This should return the address of the Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.
Datum* Scope::Find(const std::string& key)
{
	auto it = _data.find(key);
	if (it != _data.end())
	{
		return &(it->second);
	}
	return nullptr;
}
const Datum* Scope::Find(const std::string& key) const
{
	auto it = _data.find(key);
	if (it != _data.end())
	{
		return &(it->second);
	}
	return nullptr;
}

// Takes a constant string and the address of a Scope double pointer variable (whose default value could be nullptr), and which returns the address of a Datum
// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, if it exists, and nullptr otherwise
// The Scope pointer variable, if provided, shall contain the address of the Scope object which contains the match.
Datum* Scope::Search(const string& key, Scope*& scope) 
{
	auto it = _data.find(key);
	if (it != _data.end())	// key found!
	{
		scope = this;	// this current scope contains the Datum we want
		return &(it->second);
	}
	if (_parent != nullptr)	// if this scope has ancestors then keep checking
	{
		_parent->Search(key, scope);
	}
	scope = nullptr;
	return nullptr;	// the Datum associated with this key doesn't exist
}

// Clears the Data contained in this scope in this order:
// ChildScopes.Clear() -> ~ChildScopeObjects() -> vector of Datum* clear() -> vector of internal child scopes clear()
void Scope::Clear()
{
	//Scope* temp = Orphan();
	for (Datum* _datum : _vData)
	{
		if (_datum->_datType == Datum::DatumType::Table)
		{
			for (size_t i = 0; i < _datum->Size(); ++i)
			{
				Scope* s = _datum->GetScope(i);
				s->Clear();
			}
		}
	}

	for (auto it : _vInternalScopes)
	{
		delete it;
	}

	_vData.clear();	// clear our storages
	_vInternalScopes.clear();
	_data.clear();
}

// Sets this Scope's parent to the passed in newParent
void Scope::Reparent(Scope& newParent)
{
	if (_parent == nullptr)	// this scope doesn't have a child so just set it as the newParent
	{
		_parent = &newParent;	// set this child's parent as newParent
	}
	else // this scope already has a parent, so reparent responsibly
	{
		Scope* child = Orphan();
		_parent = &newParent;	// set this child's parent as newParent
	}
}

Datum* Scope::FindContainedScope(const Scope& child, size_t& idx)	// takes the constant address of a Scope and returns the Datum pointer and index at which the Scope was found.
{
	// Find child scope contained with in this "parent" scope.
	// To do that we must go through each datum in this "parent" scope to see if any of them
	// contain our child scope we want returned, if we do find it then we also update "idx" to be
	// the index that the child was found at.
	
	for (Datum* _datum : _vData)
	{
		if (_datum->_datType == Datum::DatumType::Table)
		{
			if (*_datum->GetScope() == child)	// check to see if this scope is the same as child we are looking for
			{
				idx = 0;	// keep track  how far in the scope array contained in the Datum we are then update idx to be that index
							// the reason idx = 0 here is because I currently don't have multi scope support in a single Datum TODO
				return _datum;
			}
		}
	}

	//child._parent.listOfDatums;	// cant fail
	return nullptr;
}
const Datum* Scope::FindContainedScope(const Scope& child, size_t& idx) const
{
	// find child in this.listOfDatum	// can fail
	return nullptr;
}

Datum* Scope::FindContainedScope(const std::string& childName, size_t& idx)
{
	Datum* temp = Find(childName);	// do we have this scope in our immediate children

	if (temp != nullptr)
	{
		return temp;	// we do! return it
	}

	// iterate through our map to find a Datum that holds a child scope
	for (auto it : _data)
	{
		if (it.second.DatType() == Datum::DatumType::Table)
		{
			Scope* s = it.second.GetScope();	// we found a child datum that holds a child scope
			Datum* d = s->FindContainedScope(childName, idx);	// look through this one's children
			
			return d;	// return d if it's null or not
		}
	}

	return nullptr;	// there are no child scopes in this scope
}
