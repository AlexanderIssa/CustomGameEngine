#include "pch.h"
#include "ScopeHandler.h"
#include "FactoryManager.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListWhile.h"

namespace Fiea::GameEngine
{
#pragma region ScopeHandler::ScopeWrapper
	RTTI_DEFINITIONS(ScopeHandler::ScopeWrapper);

	/// @brief Constructor that takes in a Scope ref to point at
	/// @param scopePtr 
	ScopeHandler::ScopeWrapper::ScopeWrapper(Scope& scopeRef)
	{
		_rootScopePtr = &scopeRef;	// point to this address of scope given
		_currScopePtr = &scopeRef;	// set the current scope ptr to the root scope
	}

	/**
	 * @brief RTTI override for Equals
	 * @param rhs
	 * @return true if equal, false otherwise
	*/
	bool ScopeHandler::ScopeWrapper::Equals(const RTTI* rhs) const {
		if (rhs == nullptr) return false;

		const ScopeHandler::ScopeWrapper* other = rhs->As<ScopeHandler::ScopeWrapper>();
		return maxDepth == other->maxDepth;
	}

	/**
	 * @brief RTTI override for ToString
	 * @return string representation of this Wrapper
	*/
	std::string ScopeHandler::ScopeWrapper::ToString() const {
		return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
	}

#pragma endregion ScopeHandler::ScopeWrapper

#pragma region TestParseHandler

	
	bool ScopeHandler::StartHandler(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) 
	{
		// handler can handle everything, time for a lot of if else statements!

		if (value.isArray())	// array case
		{
			// check first element to see array type
			if (value[0].isInt())	//is Json::Value array is an int array
			{
				ScopeWrapper* intWrapper = wrapper->As<ScopeWrapper>();
				if (intWrapper == nullptr) return false;	// this should never return false
				Scope* tempScope = intWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to
				Datum* tempDat = &tempScope->Append(key);	// set the datum type and add this val to it

				for (Json::ArrayIndex i = 0; i < value.size(); ++i)
				{
					int val = value[i].asInt();
					tempDat->Add(val, i);
				}
				return true;
			}
			else if (value[0].isDouble())	//is Json::Value array is an float array
			{
				ScopeWrapper* floatWrapper = wrapper->As<ScopeWrapper>();
				if (floatWrapper == nullptr) return false;	// this should never return false
				Scope* tempScope = floatWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to
				Datum* tempDat = &tempScope->Append(key);	// set the datum type and add this val to it

				for (Json::ArrayIndex i = 0; i < value.size(); ++i)
				{
					float val = value[i].asFloat();
					tempDat->Add(val, i);
				}
				return true;
			}
			else if (value[0].isString())	//is Json::Value array is an string array
			{
				// nested if times!

				string s = value[0].asString();
				string typeSuffix = key.substr(key.find_last_of('_'));	// all users MUST provide a suffix for the type if they use a string to define
																		// matrixes, vectors, or literal strings
				if (typeSuffix == "_vector")	// vector case
				{
					ScopeWrapper* vectorWrapper = wrapper->As<ScopeWrapper>();
					if (vectorWrapper == nullptr) return false;	// this should never return false
					Scope* tempScope = vectorWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to
					Datum* tempDat = &tempScope->Append(key);	// set the datum type and add this val to it

					for (Json::ArrayIndex i = 0; i < value.size(); ++i)
					{
						string val = value[i].asString();

						// can do this with datum.GetFromString()....ooops
						float f0, f1, f2, f3;
						int result = sscanf_s(val.c_str(), "(%f, %f, %f, %f)", &f0, &f1, &f2, &f3);
						if (result != 4) { throw std::invalid_argument("Invalid vector input in ScopeHandler.cpp!"); }
						glm::vec4 tempVec(f0, f1, f2, f3);

						tempDat->Add(tempVec, i);
					}
					return true;
				}
				else if (typeSuffix == "_matrix")	// matrix case
				{
					ScopeWrapper* matrixWrapper = wrapper->As<ScopeWrapper>();
					if (matrixWrapper == nullptr) return false;	// this should never return false
					Scope* tempScope = matrixWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to
					Datum* tempDat = &tempScope->Append(key);	// set the datum type and add this val to it

					for (Json::ArrayIndex i = 0; i < value.size(); ++i)
					{
						string val = value[i].asString();

						float f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15;
						int result = sscanf_s(val.c_str(), "[%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]", &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15);
						if (result != 16) { throw std::invalid_argument("Invalid input!"); }
						glm::mat4 tempMat(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);

						tempDat->Add(tempMat, i);
					}
					return true;
				}
				else	// literal string case
				{
					ScopeWrapper* stringWrapper = wrapper->As<ScopeWrapper>();
					if (stringWrapper == nullptr) return false;	// this should never return false
					Scope* tempScope = stringWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to
					Datum* tempDat = &tempScope->Append(key);	// set the datum type and add this val to it

					for (Json::ArrayIndex i = 0; i < value.size(); ++i)
					{
						string val = value[i].asString();
						tempDat->Add(val, i);
					}
					return true;
				}
			}
			else if (value[0].isObject())	// nested scope array case (NOT DONE)
			{
				//ScopeWrapper* nestedScopeWrapper = wrapper->As<ScopeWrapper>();
				//if (nestedScopeWrapper == nullptr) return false;	// this should never return false
				//Scope* tempScope = nestedScopeWrapper->GetScope();	// get the scope that the ScopeWrapper points to
				////Datum* tempDat = &tempScope->Append(key);	// set the datum type and add this val to it

				////for (Json::ArrayIndex i = 0; i < value.size(); ++i)
				////{

				////	tempDat->Add();
				////	Scope* tempScope = nestedScopeWrapper->GetScope();	// get the scope that the ScopeWrapper points to
				////	Scope& nestedScope = tempScope->AppendScope(key);	// Append a child scope with key given

				////	nestedScopeWrapper->SetCurrentScope(nestedScope);
				////}

				//for (Json::ArrayIndex i = 0; i < value.size(); ++i)
				//{
				//	StartHandler();

				//}

				return true;
			}
			return false;
		}


		if (value.isInt())	// is Json::Value is an int
		{
			ScopeWrapper* intWrapper = wrapper->As<ScopeWrapper>();
			if (intWrapper == nullptr) return false;	// this should never return false

			int val = value.asInt();
			Scope* tempScope = intWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to

			tempScope->Append(key).Add(val);	// set the datum type and add this val to it
			return true;
		}
		else if (value.isDouble())	// float case
		{
			ScopeWrapper* floatWrapper = wrapper->As<ScopeWrapper>();
			if (floatWrapper == nullptr) return false;	// this should never return false

			float val = value.asFloat();
			Scope* tempScope = floatWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to

			tempScope->Append(key).Add(val);	// set the datum type and add this val to it
			return true;
		}
		else if (value.isString())	// string case, this can be a vector, matrix, or string. Nested if time!
		{
			string s = value.asString();
			string typeSuffix = key.substr(key.find_last_of('_'));	// all users MUST provide a suffix for the type if they use a string to define matrixes, vectors, or literal strings
			string namePrefix = key.substr(0, key.find_last_of('_'));	// get the namePrefix passed in the key to seperate the name with the type

			if (typeSuffix == "_vector")	// vector case
			{
				ScopeWrapper* vectorWrapper = wrapper->As<ScopeWrapper>();
				if (vectorWrapper == nullptr) return false;	// this should never return false

				float f0, f1, f2, f3;
				int result = sscanf_s(s.c_str(), "(%f, %f, %f, %f)", &f0, &f1, &f2, &f3);
				if (result != 4) { throw std::invalid_argument("Invalid vector input in ScopeHandler.cpp!"); }
				glm::vec4 tempVec(f0, f1, f2, f3);

				Scope* tempScope = vectorWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to

				tempScope->Append(namePrefix).Add(tempVec);	// set the datum type and add this val to it
				return true;
			}
			else if (typeSuffix == "_matrix")	// matrix case
			{
				ScopeWrapper* matrixWrapper = wrapper->As<ScopeWrapper>();
				if (matrixWrapper == nullptr) return false;	// this should never return false

				float f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15;
				int result = sscanf_s(s.c_str(), "[%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]", &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15);
				if (result != 16) { throw std::invalid_argument("Invalid input!"); }
				glm::mat4 tempMat(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);

				Scope* tempScope = matrixWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to

				tempScope->Append(namePrefix).Add(tempMat);	// set the datum type and add this val to it
				return true;
			}
			else	// literal string case
			{
				ScopeWrapper* stringWrapper = wrapper->As<ScopeWrapper>();
				if (stringWrapper == nullptr) return false;	// this should never return false

				Scope* tempScope = stringWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to

				tempScope->Append(namePrefix).Add(s);	// set the datum type and add this val to it
				return true;
			}
		}
		else if (value.isObject())	// nested scope case
		{
			ScopeWrapper* nestedScopeWrapper = wrapper->As<ScopeWrapper>();
			if (nestedScopeWrapper == nullptr) return false;	// this should never return false

			string typeSuffix = key.substr(key.find_last_of('_'));
			string namePrefix = key.substr(0, key.find_last_of('_'));	// get the namePrefix passed in the key to seperate the name with the type

			if (typeSuffix == "_Scope")
			{
				Scope* tempScope = nestedScopeWrapper->GetCurrentScope();	// get the scope that the ScopeWrapper points to
				Scope& nestedScope = tempScope->AppendScope(namePrefix);	// Append a child scope with key given

				nestedScopeWrapper->SetCurrentScope(nestedScope);
				return true;
			}
			else if (typeSuffix == "_AttributedFoo")	// AttributedFoo case
			{
				Scope* temp = FactoryManager<Scope>::Create("AttributedFoo");	// create a new AttributedFoo object

				nestedScopeWrapper->GetCurrentScope()->Adopt(std::move(temp), namePrefix);	// call overloaded Adopt that takes an R-Value
				nestedScopeWrapper->SetCurrentScope(*temp);	// assign that new object as our current scope
				return true;
			}
			else if (typeSuffix == "_ChildAttributedFoo")	// AttributedFoo case
			{
				Scope* temp = FactoryManager<Scope>::Create("ChildAttributedFoo");	// create a new AttributedFoo object

				nestedScopeWrapper->GetCurrentScope()->Adopt(std::move(temp), namePrefix);	// call overloaded Adopt that takes an R-Value
				nestedScopeWrapper->SetCurrentScope(*temp);	// assign that new object as our current scope
				return true;
			}
			else if (typeSuffix == "_GameObject")
			{
				Scope* temp = FactoryManager<Scope>::Create("GameObject");	// create a new AttributedFoo object

				GameObject* GO = nestedScopeWrapper->GetCurrentScope()->As<GameObject>();
				if (GO != nullptr)
				{
					GO->AddChild(temp, namePrefix);	// if this current Scope is itself a GameObject then this nested GameObject will become its child
					
				}
				else
				{
					// we are not in a GameObject currently so its not a child GO
					nestedScopeWrapper->GetCurrentScope()->Adopt(std::move(temp), namePrefix);	// call overloaded Adopt that takes an R-Value
				}

				nestedScopeWrapper->SetCurrentScope(*temp);	// assign that new object as our current scope
				return true;
			}
			else if (typeSuffix == "_PlayerObject")
			{
				Scope* temp = FactoryManager<Scope>::Create("PlayerObject");	// create a new AttributedFoo object

				PlayerObject* PGO = nestedScopeWrapper->GetCurrentScope()->As<PlayerObject>();
				if (PGO != nullptr)
				{
					PGO->AddChild(temp, namePrefix);	// if this current Scope is itself a GameObject then this nested GameObject will become its child
				}
				else
				{
					// we are not in a GameObject currently so its not a child GO
					nestedScopeWrapper->GetCurrentScope()->Adopt(std::move(temp), namePrefix);	// call overloaded Adopt that takes an R-Value
				}

				nestedScopeWrapper->SetCurrentScope(*temp);	// assign that new object as our current scope

				return true;
			}
			else if (typeSuffix == "_ActionIncrement")
			{
				////Scope* temp = FactoryManager<Action>::Create("ActionIncrement");	// create a new AttributedFoo object
				//
				//PlayerObject* PGO = nestedScopeWrapper->GetCurrentScope()->As<PlayerObject>();
				//if (PGO != nullptr)
				//{
				//	PGO->CreateAction("ActionIncrement", namePrefix);
				//}
				//else
				//{
				//	// we are not in a GameObject currently so its not a child GO
				//	nestedScopeWrapper->GetCurrentScope()->Adopt(std::move(temp), namePrefix);	// call overloaded Adopt that takes an R-Value
				//}
				//nestedScopeWrapper->SetCurrentScope(*temp);	// assign that new object as our current scope

				//return true;
			}
			else
			{
				throw std::logic_error("Unknown Object passed in!");
			}
		}
		return false;	// type isn't a type this Handler can handle, which shouldnt exist
	}

	/**
	 * @brief Override for IParseHandler::End
	 * @param unused
	 * @param wrapper The wrapper to populate
	 * @return True, if handled, false otherwise
	*/
	bool ScopeHandler::EndHandler(const std::string& key, ParseCoordinator::Wrapper* wrapper) {
		ScopeWrapper* scopeWrapper = wrapper->As<ScopeWrapper>();
		if (scopeWrapper == nullptr) return false;

		++endCount;
		return true;
	}

}