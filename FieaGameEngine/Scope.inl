#pragma
#include "Scope.h"

namespace Fiea::GameEngine
{
	/// @brief Used to find a parent in our ancestory of a certain type passed through T
	/// @tparam T type to find
	/// @return parent of type
	template<typename T>
	T* Scope::FindParentScopeOfType()
	{
		// this scope has a parent
		if (_parent != nullptr)
		{
			// if this scope's parent is the type we want then return it
			if (_parent->As<T>() != nullptr)
			{
				return _parent->As<T>();
			}
			else
			{
				// else keep going
				return _parent->FindParentScopeOfType<T>();
			}
		}
		// until we hit the root
		return nullptr;
	}
}