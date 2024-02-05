#pragma once

#include <vector>
#include "FontParameters.h"
#include "TextBlockParameters.h"

namespace GameEngine
{
	namespace Utility
	{
		struct Utilities
		{
			static bool ReadXmlFile(const char* fileName, FontParameters& parameters);
			static bool ReadXmlFile(const char* fileName, TextBlockParameters& parameters);
		};
	}
}