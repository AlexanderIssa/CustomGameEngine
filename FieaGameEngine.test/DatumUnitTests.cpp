#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(Datumtests)
	{
	public:

		TEST_METHOD(AddAndRetreiveTest)
		{
			Datum intDat;
			Datum floatDat;
			Datum stringDat;
			Datum vecDat;
			Datum matDat;

			int32_t testInt = 1;
			float_t testFloat = 1.1f;
			string testString = "Hi there!";
			glm::vec4 tempVec(1.0f, 2.0f, 3.0f, 4.0f);
			glm::mat4 tempMat(
				1.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 1.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 1.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
			);
			intDat.Add(testInt);	// add an element, since this is the first value given to this datum it also defines that datum's type
			Assert::AreEqual(intDat.Get<int32_t>(0), testInt);
			floatDat.Add(testFloat);
			Assert::AreEqual(floatDat.Get<float_t>(0), testFloat);
			stringDat.Add(testString);
			Assert::AreEqual(stringDat.Get<string>(0) == testString, true);
			vecDat.Add(tempVec);
			Assert::AreEqual(vecDat.Get<glm::vec4>(0) == tempVec, true);
			matDat.Add(tempMat);
			Assert::AreEqual(matDat.Get<glm::mat4>(0) == tempMat, true);

			int32_t testInt2 = 2;
			float_t testFloat2 = 2.1f;
			string testString2 = "Woah! Didn't see you there ahaha";
			glm::vec4 tempVec2(3.0f, 2.0f, 1.0f, 0.0f);
			glm::mat4 tempMat2(
				2.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 3.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 4.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 5.0f  // Fourth column
			);
			intDat.Add(testInt2, 1);	// support adding post first element and at specific indexes
			Assert::AreEqual(intDat.Get<int32_t>(1), testInt2);
			floatDat.Add(testFloat2, 1);
			Assert::AreEqual(floatDat.Get<float_t>(1), testFloat2);
			stringDat.Add(testString2, 1);
			Assert::AreEqual(stringDat.Get<string>(1) == testString2, true);
			vecDat.Add(tempVec2, 1);
			Assert::AreEqual(vecDat.Get<glm::vec4>(1) == tempVec2, true);
			matDat.Add(tempMat2, 1);
			Assert::AreEqual(matDat.Get<glm::mat4>(1) == tempMat2, true);

			// if the user attemps to get a type that the Datum doesn't support, it'll send a logic error
			Assert::ExpectException<std::logic_error>([&intDat] {intDat.Get<float_t>(0); });
			Assert::ExpectException<std::logic_error>([&floatDat] {floatDat.Get<int32_t>(0); });
			Assert::ExpectException<std::logic_error>([&stringDat] {stringDat.Get<glm::vec4>(0); });
			Assert::ExpectException<std::logic_error>([&vecDat] {vecDat.Get<glm::mat4>(0); });
			Assert::ExpectException<std::logic_error>([&matDat] {matDat.Get<string>(0); });

			// if the user attempts to add an element out of bounds it'll send an out_of_range error
			Assert::ExpectException<std::out_of_range>([&intDat] {intDat.Add(12, 42); });
			Assert::ExpectException<std::out_of_range>([&floatDat] {floatDat.Add(12.2f, 42); });
			Assert::ExpectException<std::out_of_range>([&stringDat] {stringDat.Add("nice try guy!", 42); });
			Assert::ExpectException<std::out_of_range>([&vecDat, &tempVec] {vecDat.Add(tempVec, 42); });
			Assert::ExpectException<std::out_of_range>([&matDat, &tempMat] {matDat.Add(tempMat, 42); });

			// if the user attempts to get an element out of bounds it'll send an out_of_range error
			Assert::ExpectException<std::out_of_range>([&intDat] {intDat.Get<int32_t>(42); });
			Assert::ExpectException<std::out_of_range>([&floatDat] {floatDat.Get<float_t>(42); });
			Assert::ExpectException<std::out_of_range>([&stringDat] {stringDat.Get<string>(42); });
			Assert::ExpectException<std::out_of_range>([&vecDat, &tempVec] {vecDat.Get<glm::vec4>(42); });
			Assert::ExpectException<std::out_of_range>([&matDat, &tempMat] {matDat.Get<glm::mat4>(42); });
		}

		TEST_METHOD(StringManipulationAndClearTest)
		{
			Datum intDat;
			Datum floatDat;
			Datum stringDat;
			Datum vecDat;
			Datum matDat;
			glm::vec4 tempVec(1.1, 1.2, 1.3, 1.4);
			glm::mat4 tempMat(
				1.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 1.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 1.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
			);

			intDat.SetIntFromString("1");
			Assert::AreEqual(intDat.Get<int32_t>(0), 1);
			floatDat.SetFloatFromString("1.1");
			Assert::AreEqual(floatDat.Get<float_t>(0), 1.1f);
			vecDat.SetVectorFromString("(1.1, 1.2, 1.3, 1.4)");	// (%f, %f, %f, %f)
			Assert::AreEqual(vecDat.Get<glm::vec4>(0) == tempVec, true);
			matDat.SetMatrixFromString("[1.0, 0.0, 0.0, 0.0; 0.0, 1.0, 0.0, 0.0; 0.0, 0.0, 1.0, 0.0; 0.0, 0.0, 0.0, 1.0]");	// [%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]
			Assert::AreEqual(matDat.Get<glm::mat4>(0) == tempMat, true);

			glm::vec4 tempVec2(2.1, 2.2, 2.3, 2.4);
			glm::mat4 tempMat2(
				2.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 2.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 2.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 2.0f  // Fourth column
			);
			// Add an element through string without it being the first element
			intDat.SetIntFromString("2", 1);
			Assert::AreEqual(intDat.Get<int32_t>(1), 2);
			floatDat.SetFloatFromString("1.2", 1);
			Assert::AreEqual(floatDat.Get<float_t>(1), 1.2f);
			vecDat.SetVectorFromString("(2.1, 2.2, 2.3, 2.4)", 1);	// (%f, %f, %f, %f)
			Assert::AreEqual(vecDat.Get<glm::vec4>(1) == tempVec2, true);
			matDat.SetMatrixFromString("[2.0, 0.0, 0.0, 0.0; 0.0, 2.0, 0.0, 0.0; 0.0, 0.0, 2.0, 0.0; 0.0, 0.0, 0.0, 2.0]", 1);	// [%f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f; %f, %f, %f, %f]
			Assert::AreEqual(matDat.Get<glm::mat4>(1) == tempMat2, true);

			// Add an element out of bounds
			Assert::ExpectException<std::out_of_range>([&intDat] {intDat.SetIntFromString("2", 42); });
			Assert::ExpectException<std::out_of_range>([&floatDat] {floatDat.Add(12.2f, 42); });
			Assert::ExpectException<std::out_of_range>([&vecDat, &tempVec] {vecDat.SetVectorFromString("(1.1, 1.2, 1.3, 1.4)", 42); });
			Assert::ExpectException<std::out_of_range>([&matDat, &tempMat] {matDat.SetMatrixFromString("[1.0, 0.0, 0.0, 0.0; 0.0, 1.0, 0.0, 0.0; 0.0, 0.0, 1.0, 0.0; 0.0, 0.0, 0.0, 1.0]", 42); });

			string intString = "1";
			string floatString = "1.100000";	// need to figure out how limit the float's precision
			string vecString = "vec4(1.100000, 1.200000, 1.300000, 1.400000)";
			string matString = "mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))";
			Assert::AreEqual(intDat.GetIntToString(0), intString);
			Assert::AreEqual(floatDat.GetFloatToString(0), floatString);
			Assert::AreEqual(vecDat.GetVectorToString(0), vecString);
			Assert::AreEqual(matDat.GetMatrixToString(0), matString);

			Assert::ExpectException<std::out_of_range>([&intDat] {intDat.GetIntToString(42); });
			Assert::ExpectException<std::out_of_range>([&floatDat] {floatDat.GetFloatToString(42); });
			Assert::ExpectException<std::out_of_range>([&vecDat] {vecDat.GetVectorToString(42); });
			Assert::ExpectException<std::out_of_range>([&matDat] {matDat.GetMatrixToString(42); });

			// this causes stomping errors for double deleting
			//intDat.Clear();
			//Assert::AreEqual(intDat.Size(), size_t(0));
			//floatDat.Clear();
			//Assert::AreEqual(floatDat.Size(), size_t(0));
			//stringDat.Add("Clear me pls!");
			//stringDat.Clear();
			//Assert::AreEqual(stringDat.Size(), size_t(0));
			//vecDat.Clear();
			//Assert::AreEqual(vecDat.Size(), size_t(0));
			//matDat.Clear();
			//Assert::AreEqual(matDat.Size(), size_t(0));
		}

		TEST_METHOD(OperatorOverrides)
		{
			Datum intDat;
			Datum floatDat;
			Datum stringDat;
			Datum vecDat;
			Datum matDat;
			glm::vec4 tempVec(1.1, 1.2, 1.3, 1.4);
			glm::mat4 tempMat(
				1.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 1.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 1.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
			);

			intDat.Add(1);
			Datum iCopiedDat(intDat);	// copy contructor by value (deep copy)
			Assert::AreEqual(intDat == iCopiedDat, true);
			Assert::AreEqual(intDat != iCopiedDat, false);

			floatDat.Add(1.1f);
			Datum fCopiedDat(floatDat);
			Assert::AreEqual(floatDat == fCopiedDat, true);
			Assert::AreEqual(floatDat != fCopiedDat, false);

			stringDat.Add("Copy time!!");
			Datum sCopiedDat(stringDat);
			Assert::AreEqual(stringDat == sCopiedDat, true);
			Assert::AreEqual(stringDat != sCopiedDat, false);

			vecDat.Add(tempVec);
			Datum vCopiedDat(vecDat);
			Assert::AreEqual(vecDat == vCopiedDat, true);
			Assert::AreEqual(vecDat != vCopiedDat, false);

			matDat.Add(tempMat);
			Datum mCopiedDat(matDat);
			Assert::AreEqual(matDat == mCopiedDat, true);
			Assert::AreEqual(matDat != mCopiedDat, false);

			Datum intDat2;
			intDat2.Add(2);
			intDat = intDat2;	// assignment operator
			//Assert::AreEqual(intDat == intDat2, true);
			//Assert::AreEqual(intDat != iCopiedDat, true);

			//Datum floatDat2;
			//floatDat2.Add(2.2f);
			//floatDat = floatDat2;
			//Assert::AreEqual(floatDat == floatDat2, true);
			//Assert::AreEqual(floatDat != fCopiedDat, true);

			//Datum stringDat2;
			//stringDat2.Add("Oho! Here we go again!");
			//stringDat = stringDat2;
			//Assert::AreEqual(stringDat == stringDat2, true);
			//Assert::AreEqual(stringDat != sCopiedDat, true);

			//glm::vec4 tempVec2(2.1, 2.2, 2.3, 2.4);
			//Datum vecDat2;
			//vecDat2.Add(tempVec2);
			//vecDat = vecDat2;
			//Assert::AreEqual(vecDat == vecDat2, true);
			//Assert::AreEqual(vecDat != vCopiedDat, true);

			//glm::mat4 tempMat2(
			//	2.0f, 0.0f, 0.0f, 0.0f, // First column
			//	0.0f, 2.0f, 0.0f, 0.0f, // Second column
			//	0.0f, 0.0f, 2.0f, 0.0f, // Third column
			//	0.0f, 0.0f, 0.0f, 2.0f  // Fourth column
			//);
			//Datum matDat2;
			//matDat2.Add(tempMat2);
			//matDat = matDat2;
			//Assert::AreEqual(matDat == matDat2, true);
			//Assert::AreEqual(matDat != mCopiedDat, true);

			//Datum intDat3;
			//intDat3 = 1;	// scalar assignment overload
			//Assert::IsTrue(intDat3.Get<int32_t>(0) == 1);
			//Datum floatDat3;
			//floatDat3 = 1.1f;	// scalar assignment overload
			//Assert::IsTrue(floatDat3.Get<float_t>(0) == 1.1f);
			//Datum stringDat3;
			//stringDat3 = "Hi i'm a scalar! Im so scaled.";	// scalar assignment overload
			//Assert::IsTrue(stringDat3.Get<string>(0) == "Hi i'm a scalar! Im so scaled.");
			//Datum vecDat3;
			//vecDat3 = tempVec;	// scalar assignment overload
			//Assert::IsTrue(vecDat3.Get<glm::vec4>(0) == tempVec);
			//Datum matDat3;
			//matDat3 = tempMat;	// scalar assignment overload
			//Assert::IsTrue(matDat3.Get<glm::mat4>(0) == tempMat);
		}
		TEST_METHOD(CopyContructorTest)
		{
			Datum intDat;
			Datum floatDat;
			Datum stringDat;
			Datum vecDat;
			Datum matDat;
			glm::vec4 tempVec(1.1, 1.2, 1.3, 1.4);
			glm::mat4 tempMat(
				1.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 1.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 1.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
			);

			intDat.Add(1);
			Datum iCopiedDat(intDat);	// copy contructor by value (deep copy)
			Assert::AreEqual(intDat == iCopiedDat, true);

			floatDat.Add(1.1f);
			Datum fCopiedDat(floatDat);
			Assert::AreEqual(floatDat == fCopiedDat, true);

			stringDat.Add("Copy time!!");
			Datum sCopiedDat(stringDat);
			Assert::AreEqual(stringDat == sCopiedDat, true);

			vecDat.Add(tempVec);
			Datum vCopiedDat(vecDat);
			Assert::AreEqual(vecDat == vCopiedDat, true);

			matDat.Add(tempMat);
			Datum mCopiedDat(matDat);
			Assert::AreEqual(matDat == mCopiedDat, true);
		}

		TEST_METHOD(RemoveTest)
		{
			Datum intDat;
			Datum floatDat;
			Datum stringDat;
			Datum vecDat;
			Datum matDat;
			glm::vec4 tempVec(1.1, 1.2, 1.3, 1.4);
			glm::vec4 tempVec2(2.1, 2.2, 2.3, 2.4);
			glm::mat4 tempMat(
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

			intDat.Add(1);
			intDat.Add(2, 1);
			intDat.Remove(0);	// remove the element at the index
			Assert::IsTrue(intDat.Size() == (size_t)1);
			Assert::IsTrue(intDat.Get<int32_t>(0) == 2);

			floatDat.Add(1.1f);
			floatDat.Add(1.2f, 1);
			floatDat.Add(1.3f, 2);
			floatDat.Remove(1);
			Assert::IsTrue(floatDat.Size() == (size_t)2);
			Assert::IsTrue(floatDat.Get<float_t>(1) == 1.3f);

			stringDat.Add("Hello");
			stringDat.Add("Pizza", 1);
			stringDat.Add("World!", 2);
			stringDat.Remove(1);
			Assert::IsTrue(stringDat.Size() == (size_t)2);
			Assert::IsTrue(strcmp((stringDat.Get<string>(1)).c_str(), "Pizza") == 1);

			vecDat.Add(tempVec);
			vecDat.Add(tempVec2, 1);
			vecDat.Remove(0);
			Assert::IsTrue(vecDat.Size() == (size_t)1);
			Assert::IsTrue(vecDat.Get<glm::vec4>(0) == tempVec2);

			matDat.Add(tempMat);
			matDat.Add(tempMat2, 1);
			matDat.Remove(0);
			Assert::IsTrue(matDat.Size() == (size_t)1);
			Assert::IsTrue(matDat.Get<glm::mat4>(0) == tempMat2);
		}

		TEST_METHOD(SetStorageTest)
		{
			Datum intDat;
			Datum floatDat;
			Datum stringDat;
			Datum vecDat;
			Datum matDat;
			glm::vec4 tempVec(1.1, 1.2, 1.3, 1.4);
			glm::mat4 tempMat(
				1.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 1.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 1.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
			);

			int32_t* iExternalArr = new int32_t[10];
			float_t* fExternalArr = new float_t[10];
			string* sExternalArr = new string[10];
			glm::vec4* vExternalArr = new glm::vec4[10];
			glm::mat4* mExternalArr = new glm::mat4[10];

			intDat.SetStorage(iExternalArr, 10, 10);
			Assert::AreEqual(intDat.Size(), (size_t)10);
			floatDat.SetStorage(fExternalArr, 10, 10);
			Assert::AreEqual(floatDat.Size(), (size_t)10);
			stringDat.SetStorage(sExternalArr, 10, 10);
			Assert::AreEqual(stringDat.Size(), (size_t)10);
			vecDat.SetStorage(vExternalArr, 10, 10);
			Assert::AreEqual(vecDat.Size(), (size_t)10);
			matDat.SetStorage(mExternalArr, 10, 10);
			Assert::AreEqual(matDat.Size(), (size_t)10);
			intDat.~Datum();
			Assert::AreEqual(iExternalArr == nullptr, false);	// deconstructing the Datum doesn't delete the memory
			floatDat.~Datum();
			Assert::AreEqual(fExternalArr == nullptr, false);
			stringDat.~Datum();
			Assert::AreEqual(sExternalArr == nullptr, false);
			vecDat.~Datum();
			Assert::AreEqual(vExternalArr == nullptr, false);
			matDat.~Datum();
			Assert::AreEqual(mExternalArr == nullptr, false);

			delete[] iExternalArr;
			delete[] fExternalArr;
			delete[] sExternalArr;
			delete[] vExternalArr;
			delete[] mExternalArr;
		}

		TEST_METHOD(SetSizeTest)
		{
			Datum intDat;
			Datum floatDat;
			Datum stringDat;
			Datum vecDat;
			Datum matDat;
			glm::vec4 tempVec(1.1, 1.2, 1.3, 1.4);
			glm::mat4 tempMat(
				1.0f, 0.0f, 0.0f, 0.0f, // First column
				0.0f, 1.0f, 0.0f, 0.0f, // Second column
				0.0f, 0.0f, 1.0f, 0.0f, // Third column
				0.0f, 0.0f, 0.0f, 1.0f  // Fourth column
			);
			intDat.Add(1);
			intDat.Add(2, 1);
			intDat.Add(3, 2);
			intDat.SetSize<int32_t>(20);	// default size is 10 so it expands
			Assert::IsTrue(intDat.Size() == (size_t)20);

			floatDat.Add(1.1f);
			floatDat.SetSize<float_t>(20);
			Assert::IsTrue(floatDat.Size() == (size_t)20);

			stringDat.Add("Expand me!");
			stringDat.SetSize<string>(20);
			Assert::IsTrue(stringDat.Size() == (size_t)20);

			vecDat.Add(tempVec);
			vecDat.SetSize<glm::vec4>(20);
			Assert::IsTrue(vecDat.Size() == (size_t)20);

			matDat.Add(tempMat);
			matDat.SetSize<glm::mat4>(20);
			Assert::IsTrue(matDat.Size() == (size_t)20);
		}

		TEST_METHOD(ConstructorTest)
		{
			Datum intDat;
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