#pragma once

#include "GameObject.h"

namespace Fiea::GameEngine
{
	class PlayerObject : public GameObject
	{
		RTTI_DECLARATIONS(PlayerObject, GameObject);

	public:
		/// @brief Default constructor, sends ID to GameObject who sends it to Attributed
		PlayerObject();

		/// @brief PlayerObject's Update override
		/// @param time 
		void Update(const size_t& time) override;

		/// @brief Function to get this class' signatures
		/// @return 
		static std::vector<Signature> Signatures();

		std::string PlayerName = "";
//ron nya!!!!
	private:

	};

	MAKE_FACTORY(Scope, PlayerObject);
}

