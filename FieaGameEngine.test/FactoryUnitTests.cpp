#include "pch.h"
#include "CppUnitTest.h"
#include "ParseCoordinator.h"
#include "TestParseHandler.h"
#include "TestIntWrapper.h"
#include "TestIntHandler.h"
#include "ScopeHandler.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "FactoryManager.h"
#include "ChildAttributedFoo.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(FactoryTests)
	{
		TEST_METHOD(FactoryTest)
		{
			AttributedFooFactory af_Factory;	// create an instance of AttributedFooFactory
			std::unique_ptr<IFactory<Scope>> factoryPtr = std::make_unique<AttributedFooFactory>(std::move(af_Factory));	// create a unique pointer out of af_Factory
			FactoryManager<Scope>::Add(std::move(factoryPtr));	// add that unique ptr to the factory pointer into the Factory Manager

			ChildAttributedFooFactory caf_Factory;	// create an instance of ChildAttributedFooFactory
			std::unique_ptr<IFactory<Scope>> caf_factoryPtr = std::make_unique<ChildAttributedFooFactory>(std::move(caf_Factory));	// create a unique pointer out of caf_Factory
			FactoryManager<Scope>::Add(std::move(caf_factoryPtr));	// add that unique ptr to the factory pointer into the Factory Manager

			std::string jsonStr = R"-({
					"Jojo_AttributedFoo": {
					"externalInteger":1,
					"externalFloat":1.1,
					"externalString_string":"Jojo Joestar",
					"Health":5,
					"StandName_string":"None"
					},
					"Jotaro_ChildAttributedFoo": {
					"levelsArray":[2, 3, 4],
					"name_string":"Jotaro Joestar",
					"Health":50,
					"StandName_string":"Star Platinum",
					"Enemy_string":"Dio"
					}
			})-"s;

			Scope scope;
			ScopeHandler::ScopeWrapper wrapper(scope);
			ParseCoordinator parser(wrapper);
			size_t ref = parser.AddHandler(std::make_unique<ScopeHandler>());

			bool deserialized = parser.DeserializeObject(jsonStr);
			Assert::IsTrue(deserialized);	// successful deserialization

			Assert::IsTrue(scope.Find("Jojo"s) != nullptr);	// there exists a datum in our root scope that points to the Attributed Foo
			Assert::IsFalse(scope.Find("Jojo"s) == nullptr);
			Assert::IsTrue(scope.Find("externalInteger"s) == nullptr);	// root scope doesn't contain externalInteger! our attributed Foo does!

			Scope* nestedScopePtr;
			Datum* nestedScopeDatum = scope.Search("Jojo"s, nestedScopePtr);	// get Datum that holds the atrributed foo
			nestedScopePtr = nestedScopeDatum->GetScope();	// get the literal attributed foo

			// the attributed Foo has a prescribed attribute "externalInteger" that we actually changed the value for in our JSON
			Assert::IsTrue(nestedScopePtr->Find("externalInteger"s) != nullptr);	// the attributed Foo has our passed int for its prescribed attribute
			Assert::IsTrue(nestedScopePtr->Find("externalFloat"s) != nullptr);	// the attributed Foo has our passed float for its prescribed attribute
			Assert::IsTrue(nestedScopePtr->Find("Health"s) != nullptr);	// it also has a new auxiliary attribute we defined in our json
			Assert::IsTrue(nestedScopePtr->Find("StandName"s) != nullptr);	// it also has a new auxiliary attribute we defined in our json
			
			AttributedFoo* attributedFoo = nestedScopePtr->As<AttributedFoo>();	// cast the scope* that we KNOW is an attributedFoo as an AttributedFoo to get the functions of one
			Assert::IsTrue(attributedFoo->IsAttribute("externalInteger"s));	// externalInteger is a prescribed attribute
			Assert::IsTrue(attributedFoo->IsPrescribedAttribute("externalInteger"s));
			Assert::IsTrue(attributedFoo->IsAttribute("externalFloat"s));	// externalFloat is a prescribed attribute
			Assert::IsTrue(attributedFoo->IsPrescribedAttribute("externalFloat"s));
			Assert::IsTrue(attributedFoo->IsAttribute("Health"s));	// health is an aux attribute we added through our JSON
			Assert::IsTrue(attributedFoo->IsAuxiliaryAttribute("Health"s));
			Assert::IsTrue(attributedFoo->IsAttribute("StandName"s));	// StandName is an aux attribute we added through our JSON
			Assert::IsTrue(attributedFoo->IsAuxiliaryAttribute("StandName"s));

			Assert::IsTrue(attributedFoo->externalInteger == 1);	// the value we passed in through JSON two way binds the prescribed attribute's value
			Assert::IsTrue(attributedFoo->externalFloat == 1.1f);	// the value we passed in through JSON two way binds the prescribed attribute's value
			Assert::IsTrue(attributedFoo->externalString == "Jojo Joestar"s);	// the value we passed in through JSON two way binds the prescribed attribute's value
			
			attributedFoo->externalFloat = 1.2f;	// changing the value now will affect the AttributedFoo's var value
			Assert::IsTrue(attributedFoo->externalFloat == 1.2f);
			Assert::IsTrue(attributedFoo->externalFloat != 1.1f);

			Scope* nestedScopePtr2;
			Datum* nestedScopeDatum2 = scope.Search("Jotaro"s, nestedScopePtr2);	// get Datum that holds the child atrributed foo
			nestedScopePtr2 = nestedScopeDatum2->GetScope();	// get the literal child attributed foo
			ChildAttributedFoo* childAttributedFoo = nestedScopePtr2->As<ChildAttributedFoo>();

			Assert::IsTrue(nestedScopePtr2->Find("Enemy"s) != nullptr);
			Assert::IsTrue(nestedScopePtr2->Find("name"s) != nullptr);
			Assert::IsTrue(nestedScopePtr2->Find("levelsArray"s) != nullptr);
			Assert::IsTrue(nestedScopePtr2->Find("levels"s) == nullptr);
			Assert::IsTrue(childAttributedFoo->IsAttribute("name"s));	// name is a prescribed attribute
			Assert::IsTrue(childAttributedFoo->IsPrescribedAttribute("name"s));	// name is a prescribed attribute
			Assert::IsTrue(childAttributedFoo->name == "Jotaro Joestar"s);	// it was changed to our json's value
			Assert::IsTrue(childAttributedFoo->name != "Jojo"s);	// not the default value

			Assert::IsTrue(childAttributedFoo->levelsArray[0] == 2);	// not the default value
			Assert::IsTrue(childAttributedFoo->levelsArray[1] == 3);	// not the default value
			Assert::IsTrue(childAttributedFoo->levelsArray[2] == 4);	// not the default value

			childAttributedFoo->name = "Jolene Joestar";
			Assert::IsTrue(childAttributedFoo->name == "Jolene Joestar"s);	// two way binding

			// cleanup memory
			FactoryManager<Scope>::Cleanup();
		}

		TEST_METHOD(aa)
		{

		}

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			// create singleton instance
			TypeRegistry::CreateInstance();
			TypeRegistry::instance->PopulateMap(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
			TypeRegistry::instance->PopulateMap(ChildAttributedFoo::TypeIdClass(), ChildAttributedFoo::FillSignatures());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			// delete singleton instance
			TypeRegistry::instance->DeleteInstance();
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

	private:
		inline static _CrtMemState _startMemState;	// in addition to declaring it, inline also gives storage for it as well
	};
}