#pragma once
#include <string>
#include "Datum.h"

namespace Fiea::GameEngine
{
	class Signature
	{
	public:
		std::string name;
		Datum::DatumType type;
		size_t size;
		size_t offset;
	};
}