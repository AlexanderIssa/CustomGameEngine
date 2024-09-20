#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"
#include "Scope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(ScopeUnitTests)
	{
		TEST_METHOD(AdoptTest)
		{
			Scope s1;	// default constructs scope capacity to 0

			Scope* s2 = &s1.AppendScope("a"s);
			Scope* s3 = &s2->AppendScope("b"s);

			//Scope* s4 = new Scope(*s2);
			Scope* s4(s2);
			Assert::IsTrue(s4 == s2);
			Scope s5(*s3);// make a copy of s3, call it s5
			Assert::IsTrue(s5 != *s3);	// the reason they are not equal is because copy constructor does not bring over the parent
			// parent = null or parent = s3's parent (s2)
			s5.Adopt(*s4, "s4"s);// s5.parent = s4 (append scope or adopt)

			s3->Reparent(s1);	// change s3's parent to s1
			Assert::IsTrue(s3->GetParent() == s1);

			s3->Reparent(s5);	// chnage s3's parent to s5
			Assert::IsTrue(s3->GetParent() == s5);
		}

		TEST_METHOD(AppendScopeTest2)
		{
			Scope s1;
			string childName1 = "Bob";
			string childName2 = "Sam";
			string childName3 = "Alex";

			Scope* BobScope = &s1.AppendScope(childName1); // s1 has a child scope named "Bob", contained in a Datum in s1's map with "Bob" as the key
			Scope* childScopePtr = s1.Find(childName1)->GetScope();	// childScopePtr points at "Bob" Scope
			Scope* SamScope = &childScopePtr->AppendScope(childName2);	// "Bob" Scope has a child named "Sam"

			Assert::IsTrue(nullptr != childScopePtr);	// childScopePtr points at "Bob"
			Assert::IsTrue(nullptr != childScopePtr->Find(childName2));	// "Bob" Scope has a child named "Sam"
			Assert::IsTrue(nullptr != s1.Find(childName1));	// s1 contains a child named "Bob"

			size_t temp;
			Assert::IsTrue(nullptr != s1.FindContainedScope(*BobScope, temp));
			Scope s2;
			Scope* AlexScope = &s2.AppendScope(childName3);
			Assert::IsTrue(nullptr != s2.Find(childName3));	// s2 contains a child named "Alex"

			s2 = s1;
			Assert::IsTrue(nullptr == s2.Find(childName3));
			Assert::IsTrue(nullptr != s2.Find(childName1));

			Scope s3(s2);	// copy constructor test
			Assert::IsTrue(s3 == s2);
		}

		TEST_METHOD(OperatorOverrideTests)
		{
			Scope s1;
			Scope s2;
			Scope s3;
			string childName1 = "Bob";
			string childName2 = "Sam";
			string childName3 = "Alex";
			s1[childName1]; // takes a constant string and which wraps Append, for syntactic convenience.
			Assert::IsTrue(nullptr != s1.Find(childName1));
			s1.AppendScope(childName1);
			Assert::IsTrue(nullptr != s1[(size_t)0].GetScope());	// get the Datum at index 0 of s1, then get the scope it's pointing to
			Scope* temp = nullptr;
			Assert::IsTrue(nullptr != s1.Search(childName1, temp));	// look through the whole tree for this child of name "Bob"
			
			s2.Reparent(s1);	// this sets s1 as s2's parent

			Assert::IsTrue(nullptr == s2.Search(childName1, temp));
			Assert::IsTrue(nullptr == s2.Search(childName2, temp));
			Assert::IsTrue(nullptr == s2.Search(childName3, temp));
			Assert::IsTrue(s2.GetParent() == s1);
			Assert::IsFalse(s3 == s2);	// s3 and s2 have different parents
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