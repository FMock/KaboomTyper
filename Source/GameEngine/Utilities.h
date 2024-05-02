#pragma once

#include <vector>
#include <unordered_map>
#include "FontParameters.h"
#include "TextBlockParameters.h"
#include "ColorParameters.h"

namespace GameEngine
{
	namespace Utility
	{
		struct Utilities
		{
			static bool ReadXmlFile(const char* fileName, FontParameters& parameters);
			static bool ReadXmlFile(const char* fileName, TextBlockParameters& parameters);
			static bool ReadXmlFile(const char* fileName, ColorParameters& parameters);
			static bool ReadWordsFromFile(const std::string& filename, std::unordered_map<std::string, std::vector<std::string>>& words, std::vector<std::string>& categories);
		};
	}
}