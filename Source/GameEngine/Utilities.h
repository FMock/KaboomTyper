#pragma once

#include <vector>
#include <string>
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

			/// <summary>
			/// ReadConfigFileGetPath searches text file for target and returns target value if found, otherwise an empty string.
			/// </summary>
			/// <param name="filePath">path and name of text file std::string</param>
			/// <param name="target">target std::string</param>
			/// <returns>returns target value std::string if found, or an empty std::string otherwise</returns>
			static std::string ReadConfigFileGetPath(const std::string& filePath, const std::string& target);
		};
	}
}