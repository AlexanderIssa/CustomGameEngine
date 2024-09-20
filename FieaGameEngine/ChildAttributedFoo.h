#pragma once

#include "AttributedFoo.h"

namespace Fiea::GameEngine
{
	class ChildAttributedFoo : public AttributedFoo
	{
		RTTI_DECLARATIONS(ChildAttributedFoo, AttributedFoo);
	public:
		/// @brief Default constructor
		ChildAttributedFoo();

		/// @brief default destructor
		virtual ~ChildAttributedFoo() = default;

		/// @brief Fill my parents signatures with my signatures
		/// @return the combined signatures vector
		static std::vector<Signature> FillSignatures();

		/// @brief Get this class' signatures
		/// @return vector of signatures
		static std::vector<Signature> Signatures();

		int health = 100;
		float mana = 50.50;
		std::string name = "Jojo";
		glm::vec4 forwardVector;
		glm::mat4 theMatrix;

		static const std::size_t arraySize = 5;
		int levelsArray[arraySize] { 1, 2, 3 };
	};

	MAKE_FACTORY(Scope, ChildAttributedFoo);
}