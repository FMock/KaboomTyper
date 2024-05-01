#pragma once

#include "TextBlock.h"
#include "Common.h"
#include <memory>

namespace GameEngine
{
	class TextBlockGenerator
	{
	public:
		TextBlockGenerator();
		~TextBlockGenerator();
		void NextTextBlock();

	private:
		std::vector<std::unique_ptr<TextBlock>> m_textBlockContainer;
		std::string GetNextString();
	};
}