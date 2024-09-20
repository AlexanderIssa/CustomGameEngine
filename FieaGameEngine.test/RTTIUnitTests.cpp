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
	TEST_CLASS(RTTIUnitTests)
	{
		TEST_METHOD(IsTest)
		{
			//RTTI test();
			//Scope scope();
			//scope.Is();
		}

		TEST_METHOD(AsTest)
		{

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