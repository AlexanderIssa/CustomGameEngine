#pragma once
#include "IFactory.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace Fiea::GameEngine
{
	template<class T>
	class FactoryManager
	{
	public:
		/// @brief Given a class name (string), return the associated concrete factory. This runs in constant time.
		/// @param className 
		/// @return associated concrete factory
		static IFactory<T>& Find(std::string className);

		/// @brief Given a class name (string), return a new object of that type. This runs in constant time
		/// @param className 
		/// @return a new object of that type
		static T* Create(std::string className);

		/// @brief Given a reference to a concrete factory, add it to the list of factories for this abstract factory.
		/// @param concreteFactory 
		static void Add(std::unique_ptr<IFactory<T>>&& concreteFactory);

		/// @brief Given a reference to a concrete factory, remove it from the list of factories for this abstract factory.
		/// @param concreteFactory 
		static void Remove(IFactory<T>& concreteFactory);

		static void Cleanup();

	private:
		//inline static std::unordered_map<std::string, IFactory<T>*> _factories{};
		//inline static vector<IFactory<T>*> _vInstances{};
		inline static std::unordered_map<std::string, std::unique_ptr<IFactory<T>>> _factories{};
	};

}
#include "FactoryManager.inl"