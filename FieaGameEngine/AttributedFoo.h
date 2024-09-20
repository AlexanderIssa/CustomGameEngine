#pragma once

#include "FnewMacro.h"
#include "Attributed.h"
#include "Signature.h"
#include "Foo.h"
#include "IFactory.h"
#include <vector>

namespace Fiea::GameEngine
{
	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);
	public:
		AttributedFoo();
		virtual ~AttributedFoo() = default;
		AttributedFoo(const AttributedFoo& rhs) = default;
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;	// move constructor
		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;
		[[nodiscard]] AttributedFoo* Clone() const override;

		AttributedFoo(size_t);

		bool operator==(const AttributedFoo& rhs) const;

		static std::vector<Signature> Signatures();
		//void PopulateScopePrescribed();

		//// overrides of pure virtuals
		//bool IsAttribute(const std::string Name) override;
		//bool IsPrescribedAttribute(const std::string Name) override;
		//bool IsAuxiliaryAttribute(const std::string Name) override;
		//Datum& AppendAuxiliaryAttribute(const std::string Name) override;

		int externalInteger = 0;
		float externalFloat = 0;
		std::string externalString;
		glm::vec4 externalVector;
		glm::mat4 externalMatrix;
		//Foo* externalPointer;

		// Static external arrays
		static const std::size_t arraySize = 5;
		int externalIntegerArray[arraySize];
		float externalFloatArray[arraySize];
		std::string externalStringArray[arraySize];
		glm::vec4 externalVectorArray[arraySize];
		glm::mat4 externalMatrixArray[arraySize];
		//Foo* externalPointerArray[arraySize];

		std::string ToString() const override { return Attributed::ToString(); }
		//AttributedFoo* Clone() const { return FNEW AttributedFoo(*this); }

	private:

	};

	MAKE_FACTORY(Scope, AttributedFoo);
}


