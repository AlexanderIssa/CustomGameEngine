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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(ParseCoordinatorTests)
	{
		TEST_METHOD(IntHandlerWrapperTest)
		{
			std::string jsonStr = R"({
					"health_int":2
			})"s;
			TestIntWrapper wrapper;
			ParseCoordinator parser(wrapper);
			size_t ref = parser.AddHandler(std::make_unique<TestIntHandler>());

			bool deserialized = parser.DeserializeObject(jsonStr);
			Assert::IsTrue(deserialized);
		}

		TEST_METHOD(SingleScopeHandlerWrapperTest)
		{
			std::string jsonStr = R"-({
					"Health": 2,
					"Mana": 100,
					"SprintSpeed": 2.5,
					"TestVec_vector": "(1.1, 1.2, 1.3, 1.4)",
					"TestMat_matrix": "[1.0, 0.0, 0.0, 0.0; 0.0, 1.0, 0.0, 0.0; 0.0, 0.0, 1.0, 0.0; 0.0, 0.0, 0.0, 1.0]",
					"Name_string": "Alex!"
			})-"s;
			
			Scope scope;
			ScopeHandler::ScopeWrapper wrapper(scope);
			ParseCoordinator parser(wrapper);
			size_t ref = parser.AddHandler(std::make_unique<ScopeHandler>());

			bool deserialized = parser.DeserializeObject(jsonStr);
			Assert::IsTrue(deserialized);	// successful deserialization

			// All json values were converted to Datums with key of json key 
			Assert::IsTrue(scope.Find("Health"s) != nullptr);
			Assert::IsFalse(scope.Find("Health"s) == nullptr);
			Assert::IsTrue(scope.Find("Mana"s) != nullptr);
			Assert::IsFalse(scope.Find("Mana"s) == nullptr);
			Assert::IsTrue(scope.Find("SprintSpeed"s) != nullptr);
			Assert::IsFalse(scope.Find("SprintSpeed"s) == nullptr);
			Assert::IsTrue(scope.Find("TestVec"s) != nullptr);
			Assert::IsFalse(scope.Find("TestVec"s) == nullptr);
			Assert::IsTrue(scope.Find("TestMat"s) != nullptr);
			Assert::IsFalse(scope.Find("TestMat"s) == nullptr);
			Assert::IsTrue(scope.Find("Name"s) != nullptr);
			Assert::IsFalse(scope.Find("Name"s) == nullptr);
		}

		TEST_METHOD(NestedObjectTest)
		{
			std::string jsonStr = R"-({
					"Health": 2,
					"Mana": 100,
					"SprintSpeed": 2.5,
					"TestVec_vector": "(1.1, 1.2, 1.3, 1.4)",
					"TestMat_matrix": "[1.0, 0.0, 0.0, 0.0; 0.0, 1.0, 0.0, 0.0; 0.0, 0.0, 1.0, 0.0; 0.0, 0.0, 0.0, 1.0]",
					"Name_string": "Alex!",
					"NestedScope_Scope": {
						"Age": 24,
						"LevelExp": 50.5,
						"ForwardVec_vector": "(1.1, 1.2, 1.3, 1.4)",
						"TheMat_matrix": "[1.0, 0.0, 0.0, 0.0; 0.0, 1.0, 0.0, 0.0; 0.0, 0.0, 1.0, 0.0; 0.0, 0.0, 0.0, 1.0]",
						"Class_string": "Mage"
					}
			})-"s;

			Scope scope;
			ScopeHandler::ScopeWrapper wrapper(scope);
			ParseCoordinator parser(wrapper);
			size_t ref = parser.AddHandler(std::make_unique<ScopeHandler>());

			bool deserialized = parser.DeserializeObject(jsonStr);
			Assert::IsTrue(deserialized);	// successful deserialization

			// All json values were converted to Datums with key of json key 
			Assert::IsTrue(scope.Find("Health"s) != nullptr);
			Assert::IsFalse(scope.Find("Health"s) == nullptr);
			Assert::IsTrue(scope.Find("Mana"s) != nullptr);
			Assert::IsFalse(scope.Find("Mana"s) == nullptr);
			Assert::IsTrue(scope.Find("SprintSpeed"s) != nullptr);
			Assert::IsFalse(scope.Find("SprintSpeed"s) == nullptr);
			Assert::IsTrue(scope.Find("TestVec"s) != nullptr);
			Assert::IsFalse(scope.Find("TestVec"s) == nullptr);
			Assert::IsTrue(scope.Find("TestMat"s) != nullptr);
			Assert::IsFalse(scope.Find("TestMat"s) == nullptr);
			Assert::IsTrue(scope.Find("Name"s) != nullptr);
			Assert::IsFalse(scope.Find("Name"s) == nullptr);

			// Nested Scope appened to our outer scope
			Assert::IsTrue(scope.Find("NestedScope"s) != nullptr);
			Assert::IsFalse(scope.Find("Age"s) != nullptr);	// this Datum is in our nested scope so it wont show up in our parent scope!
			Assert::IsFalse(scope.Find("LevelExp"s) != nullptr);	// this Datum is in our nested scope so it wont show up in our parent scope!
			Assert::IsFalse(scope.Find("ForwardVec"s) != nullptr);	// this Datum is in our nested scope so it wont show up in our parent scope!
			Assert::IsFalse(scope.Find("TheMat"s) != nullptr);	// this Datum is in our nested scope so it wont show up in our parent scope!
			Assert::IsFalse(scope.Find("Class"s) != nullptr);	// this Datum is in our nested scope so it wont show up in our parent scope!

			Scope* nestedScopePtr;
			Datum* nestedScopeDatum = scope.Search("NestedScope"s, nestedScopePtr);	// get Datum that holds the nested scope

			nestedScopePtr = nestedScopeDatum->GetScope();	// get the literal nested scope

			// This nested scope has our nested datums in it
			Assert::IsTrue(nestedScopePtr->Find("Age"s) != nullptr);	// the nested scope contains a datum that has the int 24!
			Assert::IsTrue(nestedScopePtr->Find("LevelExp"s) != nullptr);	// the nested scope contains a float that has the float 50.5!
			Assert::IsTrue(nestedScopePtr->Find("ForwardVec"s) != nullptr);
			Assert::IsTrue(nestedScopePtr->Find("TheMat"s) != nullptr);
			Assert::IsTrue(nestedScopePtr->Find("Class"s) != nullptr);
		}

		TEST_METHOD(JsonArrayTest)
		{
			std::string jsonStr = R"-({
					"IntArray_int": [1, 2, 3],
					"FloatArray_float": [1.1, 2.2, 3.3],
					"VectorArray_vector": ["(1.1, 1.2, 1.3, 1.4)", "(2.1, 2.2, 2.3, 2.4)", "(3.1, 3.2, 3.3, 3.4)"],
					"MatrixArray_matrix": ["[1.0, 0.0, 0.0, 0.0; 0.0, 1.0, 0.0, 0.0; 0.0, 0.0, 1.0, 0.0; 0.0, 0.0, 0.0, 1.0]", "[2.0, 0.0, 0.0, 0.0; 0.0, 2.0, 0.0, 0.0; 0.0, 0.0, 2.0, 0.0; 0.0, 0.0, 0.0, 2.0]", "[3.0, 0.0, 0.0, 0.0; 0.0, 3.0, 0.0, 0.0; 0.0, 0.0, 3.0, 0.0; 0.0, 0.0, 0.0, 3.0]"],
					"StringArray_string": ["Under there! ", "Underwear? ", "Haha made you say it!"]
			})-"s;


			glm::vec4 tempVec1(1.1, 1.2, 1.3, 1.4);
			glm::vec4 tempVec2(2.1, 2.2, 2.3, 2.4);
			glm::vec4 tempVec3(3.1, 3.2, 3.3, 3.4);

			glm::mat4 tempMat1(
				1.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 1.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 1.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
			);
			glm::mat4 tempMat2(
				2.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 2.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 2.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 2.0f  // Fourth column
			);
			glm::mat4 tempMat3(
				3.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 3.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 3.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 3.0f  // Fourth column
			);

			Scope scope;
			ScopeHandler::ScopeWrapper wrapper(scope);
			ParseCoordinator parser(wrapper);
			size_t ref = parser.AddHandler(std::make_unique<ScopeHandler>());

			bool deserialized = parser.DeserializeObject(jsonStr);
			Assert::IsTrue(deserialized);	// successful deserialization

			// All json values were converted to Datums with key of json key 
			Assert::IsTrue(scope.Find("IntArray_int"s) != nullptr);
			Assert::IsFalse(scope.Find("IntArray_int"s) == nullptr);
			Assert::IsTrue(scope.Find("IntArray_int"s) != nullptr);
			Assert::IsFalse(scope.Find("IntArray_int"s) == nullptr);


			Datum* intDatum = scope.Find("IntArray_int"s);
			// We have a full array in our int datum!
			Assert::IsTrue(intDatum->Get<int32_t>((size_t)0) == 1);
			Assert::IsTrue(intDatum->Get<int32_t>((size_t)1) == 2);
			Assert::IsTrue(intDatum->Get<int32_t>((size_t)2) == 3);

			Datum* floatDatum = scope.Find("FloatArray_float"s);
			// We have a full array in our float datum!
			Assert::IsTrue(floatDatum->Get<float_t>((size_t)0) == 1.1f);
			Assert::IsTrue(floatDatum->Get<float_t>((size_t)1) == 2.2f);
			Assert::IsTrue(floatDatum->Get<float_t>((size_t)2) == 3.3f);

			Datum* vectorDatum = scope.Find("VectorArray_vector"s);
			// We have a full array in our vector datum!
			Assert::IsTrue(vectorDatum->Get<glm::vec4>((size_t)0) == tempVec1);
			Assert::IsTrue(vectorDatum->Get<glm::vec4>((size_t)1) == tempVec2);
			Assert::IsTrue(vectorDatum->Get<glm::vec4>((size_t)2) == tempVec3);

			Datum* matrixDatum = scope.Find("MatrixArray_matrix"s);
			// We have a full array in our matrix datum!
			Assert::IsTrue(matrixDatum->Get<glm::mat4>((size_t)0) == tempMat1);
			Assert::IsTrue(matrixDatum->Get<glm::mat4>((size_t)1) == tempMat2);
			Assert::IsTrue(matrixDatum->Get<glm::mat4>((size_t)2) == tempMat3);

			Datum* stringDatum = scope.Find("StringArray_string"s);
			// We have a full array in our string datum!
			Assert::IsTrue(stringDatum->Get<string>((size_t)0) == "Under there! ");
			Assert::IsTrue(stringDatum->Get<string>((size_t)1) == "Underwear? ");
			Assert::IsTrue(stringDatum->Get<string>((size_t)2) == "Haha made you say it!");
		}

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
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