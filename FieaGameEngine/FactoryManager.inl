#pragma once

#include "FactoryManager.h"

namespace Fiea::GameEngine
{
	template<class T>
	inline IFactory<T>& FactoryManager<T>::Find(std::string className)
	{
		auto it = _factories.find(className);
		if (it != _factories.end())
		{
			// key found
			return *it->second;
		}
		else
		{
			// Key not found
			throw std::logic_error("Factory not found!");
		}
	}

	template<class T>
	inline T* FactoryManager<T>::Create(std::string className)
	{
		IFactory<T>* temp = &Find(className);	// check if we have a factory for this class
		if (temp == nullptr)
		{
			throw std::logic_error("this factory doesn't exist");	// nope, we dont :(
		}

		//IFactory<T>* tempPtr = temp->Create();

		return temp->Create();	// yes we do :) create the instance of it
	}

	template<class T>
	inline void FactoryManager<T>::Add(std::unique_ptr<IFactory<T>>&& concreteFactory)
	{
		_factories[concreteFactory->ClassName()] = std::move(concreteFactory);	// add concrete factory using it's name into our map

		//IFactory<T>* temp = &Find(concreteFactory->ClassName());
		//if (temp == nullptr)	// we don't already have it
		//{
		//	_factories[concreteFactory->ClassName()] = std::move(concreteFactory);	// add concrete factory using it's name into our map
		//}
		// if we already have it then no need to add it again
	}

	template<class T>
	inline void FactoryManager<T>::Remove(IFactory<T>& concreteFactory)
	{
		IFactory<T>* temp = &Find(concreteFactory.ClassName());
		if (temp != nullptr)	// we have this key in our map
		{
			// erase it
			_factories.erase(concreteFactory.ClassName());
		}
		else
		{
			throw std::logic_error("This factory doesn't exist");
		}
	}

	template<class T>
	inline void FactoryManager<T>::Cleanup()
	{
		_factories.clear();
	}
}