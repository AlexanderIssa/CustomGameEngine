#pragma once

#include "FnewMacro.h"
#include "RTTI.h"
#include "Datum.h"
#include <unordered_map>

namespace Fiea::GameEngine
{
	// Scope objects are tables that create dictionary of name-value pairs where Datum objects are the values.
	// Scopes are tables of Datum, some of which can be other tables (i.e. Scopes).  Also, since each Scope has a pointer to its parent, this forms a tree of Scopes. 
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);
	public:
		Scope(size_t cap = 0); // default constructor
		Scope(const Scope& other); // copy constructor
		Scope(Scope&& other) noexcept;	// move constructor
		virtual ~Scope();

		// Operator overrides
		Scope& operator=(const Scope& rhs);
		Scope& operator=(Scope && rhs) noexcept;
		bool operator==(const Scope& rhs) const;
		bool operator!=(const Scope& rhs) const;
		Datum& operator[](const string key);	// should be implemented with Append(), can't guarentee that Append() won't mess with our data so we dont have a const version
		Datum& operator[](size_t idx);
		const Datum& operator[](size_t idx) const;

		[[nodiscard]] virtual Scope* Clone() const;	// no discard makes sure we don't discard memory and leak

		Datum* Find(const std::string& key);	// if it can't find it, returns nullptr
		const Datum* Find(const std::string& key) const;
		Datum& Append(const std::string& key);	// Appends a new Datum into the map using the passed in string as the key
		Scope& AppendScope(const std::string& key);	// "Birth" a child scope in a Datum and that Datum owns the child (internal memory)
		void Adopt(Scope& child, const string& childName, Scope* parentOverride = nullptr);	// Adopt a literal child, Datum did not "birth" it so it doesn't "own" it (external memory)
		void Adopt(Scope*&& child, const string& childName, Scope* parentOverride = nullptr);	// Adopt overload that takes an R-Value so that this scope owns the memory of the child scope
		void DisownChild(Datum* childDatum, std::string name = "");
		void DestroyChildScopes();
		void DestroyDatums();
		
		/*static*/ Datum* FindContainedScope(const Scope& child, size_t& idx);
		const Datum* FindContainedScope(const Scope& child, size_t& idx) const;
		Datum* FindContainedScope(const std::string& child, size_t& idx);

		template<class T>
		T* FindParentScopeOfType();

		Datum* Search(const string& key);
		Datum* Search(const string& key, Scope*& scope);
		const Datum* Search(const string& key) const;
		const Datum* Search(const string& key, const Scope*& scope) const;

		// RTTI Implementation
		//virtual string ToString() const override;

		void Reparent(Scope& newParent);

		// Retrieval
		Scope& GetParent() { return *_parent; }
		Scope* GetParentPtr() { return _parent; }
		std::vector<Datum*> GetDatums() { return _vData; }
		size_t Size() { return _vData.size(); };

	protected:
		// Each entry in a Scope table has a name and a Datum, where the Datum represents an array of values of a single type.
		std::unordered_map<string, Datum> _data{};
		std::vector<Datum*> _vData{};
		std::vector<Scope*> _vInternalScopes{};
		Scope* _parent{};	// A scope's parent can only be another Scope

		void Clear();
		[[nodiscard]] Scope* Orphan();
		bool IsAncestorOf(const Scope& decendent) const;
		bool IsDescendentOf(const Scope& ancestor) const;
	};
}

#include "Scope.inl"