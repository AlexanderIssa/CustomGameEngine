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
#include "GameObject.h"
#include "PlayerObject.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(GameObjectTests)
	{
		TEST_METHOD(ConstructionTest)
		{
			PlayerObject p1;
			PlayerObject* p2 = FNEW PlayerObject();
			PlayerObject* p3 = FNEW PlayerObject();
			size_t time = 1;
			
			Assert::IsTrue(p1.PlayerName == "");	// player name is defaultly empty
			Assert::IsTrue(p2->PlayerName == "");	// player name is defaultly empty
			Assert::IsTrue(p3->PlayerName == "");	// player name is defaultly empty

			p1.AddChild(p2);
			p2->AddChild(p3); // p3 is a grandchild of p1

			p1.Update(time);	// call update on all game objects

			Assert::IsTrue(p1.PlayerName == "Update hit!"s);	// player name is now changed because update was called!
			Assert::IsTrue(p2->PlayerName == "Update hit!"s);
			Assert::IsTrue(p3->PlayerName == "Update hit!"s);

			// cleanup memory
			FactoryManager<Scope>::Cleanup();
		}

		TEST_METHOD(JsonConstructionTest)
		{
			PlayerObjectFactory p_Factory;	// create an instance of PlayerObjectFactory
			std::unique_ptr<IFactory<Scope>> factoryPtr = std::make_unique<PlayerObjectFactory>(std::move(p_Factory));	// create a unique pointer out of p_Factory
			FactoryManager<Scope>::Add(std::move(factoryPtr));	// add that unique ptr to the factory pointer into the Factory Manager
			size_t time = 1;

			glm::vec4 tempVec1(1.1, 1.2, 1.3, 1.4);

			std::string jsonStr = R"-({
					"BobPlayer_PlayerObject": {
					"PlayerName_string":"Bob",
					"TransformPos_vector": "(1.1, 1.2, 1.3, 1.4)",
					"name_string":"Bob GO",
					"BobJrPlayer_PlayerObject": {
					"PlayerName_string":"BobJr"
					}
				}
			})-"s;
			

			Scope scope;
			ScopeHandler::ScopeWrapper wrapper(scope);
			ParseCoordinator parser(wrapper);
			size_t ref = parser.AddHandler(std::make_unique<ScopeHandler>());

			bool deserialized = parser.DeserializeObject(jsonStr);
			Assert::IsTrue(deserialized);	// successful deserialization

			Assert::IsTrue(scope.Find("BobPlayer"s) != nullptr);	// there exists a datum in our root scope that points to the PlayerObject
			Assert::IsFalse(scope.Find("BobPlayer"s) == nullptr);

			Scope* nestedScopePtr;
			Datum* nestedScopeDatum = scope.Search("BobPlayer"s, nestedScopePtr);	// get Datum that holds the atrributed foo
			nestedScopePtr = nestedScopeDatum->GetScope();	// get the literal attributed foo

			PlayerObject* bobPlayerPtr = nestedScopePtr->As<PlayerObject>();
			Assert::IsTrue(bobPlayerPtr->PlayerName == "Bob");	// the name we passed in JSON replaced the BobPlayer's PlayerName
			Assert::IsTrue(bobPlayerPtr->transform.pos == tempVec1);	// the vector we passed in JSON replaced his transform.pos as well

			Assert::IsTrue(bobPlayerPtr->GetChild("BobJrPlayer"s) != nullptr);	// Bob has a child, Bob Jr
			//Datum* nestedScopeDatum2 = bobPlayerPtr->Search("BobJrPlayer"s, nestedScopePtr);
			Datum* nestedScopeDatum2 = bobPlayerPtr->GetChild("BobJrPlayer"s);
			Scope* nestedScopePtr2 = nestedScopeDatum2->GetScope();
			PlayerObject* bobJrPlayerPtr = nestedScopePtr2->As<PlayerObject>();

			Assert::IsTrue(bobJrPlayerPtr->PlayerName == "BobJr");	// Bob JR has the right name

			bobPlayerPtr->Update(time);	// run the update on Bob and all his children

			Assert::IsTrue(bobPlayerPtr->PlayerName == "Update hit!");	// Bob ran his own update!
			Assert::IsTrue(bobPlayerPtr->PlayerName != "Bob");
			Assert::IsTrue(bobJrPlayerPtr->PlayerName == "Update hit!");	// Bob's child, Bob Jr, also ran update!
			Assert::IsTrue(bobJrPlayerPtr->PlayerName != "BobJr");

			bobPlayerPtr->PlayerName = "Bob!";	// lets give him back his name with two way binding
			Assert::IsTrue(bobPlayerPtr->PlayerName == "Bob!");

			bobJrPlayerPtr->PlayerName = "BobJr!";	// lets also give BobJr his name back with two way binding
			Assert::IsTrue(bobJrPlayerPtr->PlayerName == "BobJr!");

			bobPlayerPtr->RemoveChild("BobJrPlayer");	// disown BobJr

			Assert::IsTrue(bobPlayerPtr->GetChild("BobJrPlayer"s) == nullptr);	// Bob Jr has been disowned by bob :(


			// cleanup memory
			FactoryManager<Scope>::Cleanup();
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
			TypeRegistry::instance->PopulateMap(GameObject::TypeIdClass(), GameObject::Signatures());
			TypeRegistry::instance->PopulateMap(PlayerObject::TypeIdClass(), PlayerObject::Signatures());
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