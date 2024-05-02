#pragma once

#include "TextBlock.h"
#include "Common.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include "Utilities.h"

namespace GameEngine
{
	class TextBlockManager
	{
	public:
		TextBlockManager();
		~TextBlockManager();
		void NextTextBlock();

	private:
		bool m_initialized;
		std::vector<std::unique_ptr<TextBlock>> m_textBlockContainer;
		std::unordered_map<std::string, std::vector<std::string>> m_words;
		std::vector<std::string> m_categories;
		std::string GetNextString();
	};
}