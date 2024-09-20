#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"
#include "Scope.h"
#include "pch.h"
#include "CppUnitTest.h"
#include "ParseCoordinator.h"
#include "TestParseHandler.h"
#include "TestIntWrapper.h"
#include "TestIntHandler.h"
#include "ScopeHandler.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "ChildAttributedFoo.h"
#include "FactoryManager.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include "ActionIncrement.h"
#include "GameObject.h"
#include "PlayerObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(ActionsTest)
	{
		TEST_METHOD(ActionsIncrement)
		{
			PlayerObjectFactory p_Factory;	// create an instance of PlayerObjectFactory
			std::unique_ptr<IFactory<Scope>> factoryPtr = std::make_unique<PlayerObjectFactory>(std::move(p_Factory));	// create a unique pointer out of p_Factory
			FactoryManager<Scope>::Add(std::move(factoryPtr));	// add that unique ptr to the factory pointer into the Factory Manager
			
			ActionIncrementFactory ai_Factory;
			std::unique_ptr<IFactory<Action>> factory2Ptr = std::make_unique<ActionIncrementFactory>(std::move(ai_Factory));	// create a unique pointer out of p_Factory
			FactoryManager<Action>::Add(std::move(factory2Ptr));	// add that unique ptr to the factory pointer into the Factory Manager
			
			size_t time = 1;
			size_t buffer = 0;
			Datum intDat;
			intDat.Add((int32_t)1, 0);

			PlayerObject pObj;
			pObj["intDat"] = std::move(intDat);

			pObj.CreateAction("ActionIncrement", "IncrementIntAction");
			pObj.FindContainedScope("IncrementIntAction", buffer)->GetScope()->As<ActionIncrement>()->SetKey("intDat");
			//pObj.GetAction("IncermentIntAction")->As<ActionIncrement>()->SetKey("intDat");
			pObj.Update(time);

			// cleanup memory
			FactoryManager<Scope>::Cleanup();
			FactoryManager<Action>::Cleanup();
		}

		TEST_METHOD(ActionsList)
		{
			PlayerObjectFactory p_Factory;	// create an instance of PlayerObjectFactory
			std::unique_ptr<IFactory<Scope>> factoryPtr = std::make_unique<PlayerObjectFactory>(std::move(p_Factory));	// create a unique pointer out of p_Factory
			FactoryManager<Scope>::Add(std::move(factoryPtr));	// add that unique ptr to the factory pointer into the Factory Manager

			ActionIncrementFactory ai_Factory;
			std::unique_ptr<IFactory<Action>> factory2Ptr = std::make_unique<ActionIncrementFactory>(std::move(ai_Factory));	// create a unique pointer out of p_Factory
			FactoryManager<Action>::Add(std::move(factory2Ptr));	// add that unique ptr to the factory pointer into the Factory Manager

			ActionListFactory al_Factory;
			std::unique_ptr<IFactory<Action>> factory3Ptr = std::make_unique<ActionListFactory>(std::move(al_Factory));
			FactoryManager<Action>::Add(std::move(factory3Ptr));

			size_t time = 1;
			size_t buffer = 0;
			Datum intDat;
			intDat.Add((int32_t)1, 0);
			Datum floatDat;
			floatDat.Add((float_t)2.2, 0);

			PlayerObject pObj;
			pObj["intDat"] = std::move(intDat);
			pObj["floatDat"] = std::move(floatDat);

			pObj.CreateAction("ActionList", "ListOfActions");
			ActionList* aList = pObj.FindContainedScope("ListOfActions", buffer)->GetScope()->As<ActionList>();
			aList->CreateAndAddAction("ActionIncrement", "IncrementIntAction");
			aList->CreateAndAddAction("ActionIncrement", "IncrementFloatAction");
			pObj.FindContainedScope("IncrementIntAction", buffer)->GetScope()->As<ActionIncrement>()->SetKey("intDat");
			pObj.FindContainedScope("IncrementFloatAction", buffer)->GetScope()->As<ActionIncrement>()->SetKey("floatDat");
			pObj.FindContainedScope("IncrementFloatAction", buffer)->GetScope()->As<ActionIncrement>()->SetValue(-2.2f);	// set the value of this increment

			pObj.Update(time); // update will update everything in the action list which rigt now are two increment actions

			int32_t i = pObj.Find("intDat")->Get<int32_t>(0);
			float_t f = pObj.Find("floatDat")->Get<float_t>(0);
			Assert::IsTrue(i == 2);	// after update, intDat's index 0 got incremented by IncrementIntAction's val = 1. 1+1=2
			Assert::IsTrue(f == 0);	// after update, floatDat's index 0 got decremented by IncrementFloatAction's val = -2.2. 2.2-2.2=0

			// cleanup memory
			FactoryManager<Scope>::Cleanup();
			FactoryManager<Action>::Cleanup();
		}

		TEST_METHOD(ActionsListWhile)
		{
			PlayerObjectFactory p_Factory;	// create an instance of PlayerObjectFactory
			std::unique_ptr<IFactory<Scope>> factoryPtr = std::make_unique<PlayerObjectFactory>(std::move(p_Factory));	// create a unique pointer out of p_Factory
			FactoryManager<Scope>::Add(std::move(factoryPtr));	// add that unique ptr to the factory pointer into the Factory Manager

			ActionIncrementFactory ai_Factory;
			std::unique_ptr<IFactory<Action>> factory2Ptr = std::make_unique<ActionIncrementFactory>(std::move(ai_Factory));	// create a unique pointer out of p_Factory
			FactoryManager<Action>::Add(std::move(factory2Ptr));	// add that unique ptr to the factory pointer into the Factory Manager

			ActionListFactory al_Factory;
			std::unique_ptr<IFactory<Action>> factory3Ptr = std::make_unique<ActionListFactory>(std::move(al_Factory));
			FactoryManager<Action>::Add(std::move(factory3Ptr));

			ActionListWhileFactory alw_Factory;
			std::unique_ptr<IFactory<ActionList>> factory4Ptr = std::make_unique<ActionListWhileFactory>(std::move(alw_Factory));
			FactoryManager<ActionList>::Add(std::move(factory4Ptr));

			size_t time = 1;
			size_t buffer = 0;
			Datum intDat;
			intDat.Add((int32_t)1, 0);
			Datum incrementDat;
			incrementDat.Add((int32_t)3, 0);
			Datum floatDat;
			floatDat.Add((float_t)2.2, 0);

			PlayerObject pObj;
			// give game object 3 datums that store the while loop's values
			pObj["intDat"] = std::move(intDat);
			pObj["floatDat"] = std::move(floatDat);
			pObj["incrementDat"] = std::move(incrementDat);

			// create the action list while action within the player object
			pObj.CreateAction("ActionListWhile", "WhileAction");
			ActionListWhile* aListWhile = pObj.FindContainedScope("WhileAction", buffer)->GetScope()->As<ActionListWhile>();

			// create and add actions for the ActionListWhile
			aListWhile->CreateAndAddAction("ActionIncrement", "IncrementIntAction");

			//set the keys and values of the increment actions
			aListWhile->GetAction("IncrementIntAction")->As<ActionIncrement>()->SetKey("intDat");

			// set the Increment and Preamble of the ActionListWhile to the WhileIncrement and IncrementFloatAction
			aListWhile->CreateAndSetIncrement("ActionIncrement", "WhileIncrementAction")->As<ActionIncrement>()->SetKey("incrementDat");
			aListWhile->GetIncrement()->SetValue(-1);
			aListWhile->CreateAndSetPreamble("ActionIncrement", "IncrementFloatAction")->As<ActionIncrement>()->SetKey("floatDat");
			aListWhile->GetPreamble()->As<ActionIncrement>()->SetValue(-2.2f);

			pObj.Update(time); // update will update everything in the action list in a while loop and will update the Preamble once

			// retreive the values
			int32_t i = pObj.Find("intDat")->Get<int32_t>(0);
			float_t f = pObj.Find("floatDat")->Get<float_t>(0);
			int32_t increment = pObj.Find("incrementDat")->Get<int32_t>(0);
			Assert::IsTrue(i == 4);	// after update, intDat's index 0 got incremented by IncrementIntAction's val = 1. 1+1=2
			Assert::IsTrue(f == 0);	// after update, floatDat's index 0 got decremented by IncrementFloatAction's val = -2.2. 2.2-2.2=0
			Assert::IsTrue(increment == 0);	// after update, floatDat's index 0 got decremented by IncrementFloatAction's val = -2.2. 2.2-2.2=0

			// cleanup memory
			FactoryManager<Scope>::Cleanup();
			FactoryManager<Action>::Cleanup();
			FactoryManager<ActionList>::Cleanup();
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
			TypeRegistry::instance->PopulateMap(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeRegistry::instance->PopulateMap(Action::TypeIdClass(), Action::Signatures());
			TypeRegistry::instance->PopulateMap(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeRegistry::instance->PopulateMap(ActionListWhile ::TypeIdClass(), ActionListWhile::Signatures());
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