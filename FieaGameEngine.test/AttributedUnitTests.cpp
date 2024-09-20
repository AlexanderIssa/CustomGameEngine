#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"
#include "Scope.h"
#include "AttributedFoo.h"
#include "ChildAttributedFoo.h"
#include "TypeRegistry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;

namespace FieaGameEnginetest
{
	TEST_CLASS(AttributedUnitTests)
	{
		TEST_METHOD(AttributeManipulationTest)
		{
			AttributedFoo aFoo;

			Assert::IsTrue(aFoo.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsFalse(aFoo.IsAttribute("Imposter"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute

			aFoo.AppendAuxiliaryAttribute("Fireball"s);
			Assert::ExpectException<std::logic_error>([&aFoo] { aFoo.AppendAuxiliaryAttribute("externalInteger"s); });

			Assert::IsTrue(aFoo.IsAuxiliaryAttribute("Fireball"s));	// Fireball is an Auxiliary Attribute
			Assert::IsFalse(aFoo.IsPrescribedAttribute("Fireball"s));	// which means it's not a Prescribed Attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("Imposter"s));
		}

		TEST_METHOD(CopyConstructorTest)
		{
			AttributedFoo aFoo;

			Assert::IsTrue(aFoo.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute

			aFoo.AppendAuxiliaryAttribute("Fireball"s);

			Assert::IsTrue(aFoo.IsAuxiliaryAttribute("Fireball"s));	// Fireball is an Auxiliary Attribute
			Assert::IsFalse(aFoo.IsPrescribedAttribute("Fireball"s));	// which means it's not a Prescribed Attribute

			AttributedFoo aFoo2(aFoo);

			Assert::IsTrue(aFoo2.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo2.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("externalMatrix"s));
			Assert::IsTrue(aFoo2.IsAuxiliaryAttribute("Fireball"s));	// Fireball is a Auxiliary Attribute from aFoo that copied over to aFoo2 as it's own AuxiliaryAttribute
		}

		TEST_METHOD(MoveConstructorTest)
		{
			AttributedFoo aFoo;

			Assert::IsTrue(aFoo.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute

			aFoo.AppendAuxiliaryAttribute("Fireball"s);

			Assert::IsTrue(aFoo.IsAuxiliaryAttribute("Fireball"s));	// Fireball is an Auxiliary Attribute
			Assert::IsFalse(aFoo.IsPrescribedAttribute("Fireball"s));	// which means it's not a Prescribed Attribute

			AttributedFoo aFoo2(std::move(aFoo));

			//Assert::AreSame(aFoo2.ToString(), aFoo.ToString());
			Assert::IsTrue(aFoo2.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo2.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("externalMatrix"s));
			Assert::IsTrue(aFoo2.IsAuxiliaryAttribute("Fireball"s));	// Fireball is a Auxiliary Attribute from aFoo that copied over to aFoo2 as it's own AuxiliaryAttribute
		}

		TEST_METHOD(AssignmentOperatorTest)
		{
			AttributedFoo aFoo;

			Assert::IsTrue(aFoo.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsFalse(aFoo.IsAttribute("Imposter"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute

			aFoo.AppendAuxiliaryAttribute("Fireball"s);

			Assert::IsTrue(aFoo.IsAuxiliaryAttribute("Fireball"s));	// Fireball is an Auxiliary Attribute
			Assert::IsFalse(aFoo.IsPrescribedAttribute("Fireball"s));	// which means it's not a Prescribed Attribute

			AttributedFoo aFoo2 = aFoo;

			Assert::IsTrue(aFoo.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsFalse(aFoo.IsAttribute("Imposter"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute
			Assert::IsTrue(aFoo2.IsAuxiliaryAttribute("Fireball"s));	// Fireball is an Auxiliary Attribute
			Assert::IsFalse(aFoo2.IsPrescribedAttribute("Fireball"s));	// which means it's not a Prescribed Attribute
			Assert::IsTrue(aFoo == aFoo2);

		}

		TEST_METHOD(MultiInheritanceTest)
		{
			AttributedFoo aFoo;
			ChildAttributedFoo cFoo;

			Assert::IsTrue(aFoo.IsAttribute("externalInteger"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsFalse(aFoo.IsAttribute("Imposter"s));	// externalInteger is a Prescribed Attribute which is an attribute
			Assert::IsTrue(aFoo.IsPrescribedAttribute("externalFloat"s));	// externalFloat is a Prescribed Attribute
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("externalMatrix"s));	// externalMatrix is a Prescribed Attribute so it is NOT an Auxiliary Attribute

			// our child Foo will have all of it's parents attributes PLUS it's own
			Assert::IsTrue(cFoo.IsAttribute("externalInteger"s));
			Assert::IsTrue(cFoo.IsPrescribedAttribute("externalFloat"s));
			Assert::IsFalse(cFoo.IsAuxiliaryAttribute("externalMatrix"s));
			Assert::IsTrue(cFoo.IsPrescribedAttribute("health"s));
			Assert::IsTrue(cFoo.IsPrescribedAttribute("name"s));
			Assert::IsTrue(cFoo.IsPrescribedAttribute("mana"s));
			Assert::IsTrue(cFoo.IsPrescribedAttribute("forwardVector"s));
			Assert::IsTrue(cFoo.IsPrescribedAttribute("theMatrix"s));

			// our parent's foo does not have our child's attributes
			Assert::IsFalse(aFoo.IsAttribute("health"s));
			Assert::IsFalse(aFoo.IsPrescribedAttribute("name"s));
			Assert::IsFalse(aFoo.IsPrescribedAttribute("mana"s));
			Assert::IsFalse(aFoo.IsPrescribedAttribute("forwardVector"s));
			Assert::IsFalse(aFoo.IsPrescribedAttribute("theMatrix"s));

			aFoo.AppendAuxiliaryAttribute("Fireball"s);

			Assert::IsTrue(aFoo.IsAuxiliaryAttribute("Fireball"s));	// Fireball is an Auxiliary Attribute
			Assert::IsFalse(aFoo.IsPrescribedAttribute("Fireball"s));	// which means it's not a Prescribed Attribute

			// this means that cFoo doesnt have aFoo's aux attributes
			Assert::IsFalse(cFoo.IsAttribute("Fireball"s));
			Assert::IsFalse(cFoo.IsPrescribedAttribute("Fireball"s));
			Assert::IsFalse(cFoo.IsAuxiliaryAttribute("Fireball"s));

			cFoo.AppendAuxiliaryAttribute("StarPlatinum"s);

			Assert::IsTrue(cFoo.IsAuxiliaryAttribute("StarPlatinum"s));	// StarPlatinum is an Auxiliary Attribute
			Assert::IsFalse(cFoo.IsPrescribedAttribute("StarPlatinum"s));	// which means it's not a Prescribed Attribute

			// this means that aFoo doesnt have cFoo's aux attributes
			Assert::IsFalse(aFoo.IsAttribute("StarPlatinum"s));
			Assert::IsFalse(aFoo.IsPrescribedAttribute("StarPlatinum"s));
			Assert::IsFalse(aFoo.IsAuxiliaryAttribute("StarPlatinum"s));
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