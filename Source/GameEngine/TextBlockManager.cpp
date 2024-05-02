#include "TextBlockManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace GameEngine;
using namespace GameEngine::Utility;

TextBlockManager::TextBlockManager() : m_textBlockContainer()
{
	m_initialized = Utilities::ReadWordsFromFile(R"(../../Config/words.txt)", m_words, m_categories);
}

TextBlockManager::~TextBlockManager()
{
}

void TextBlockManager::NextTextBlock()
{
	if(!m_initialized)
		std::cerr << "Failed to open words file. " << std::endl;

	std::string nextStr = GetNextString();
	m_textBlockContainer.push_back(std::make_unique<TextBlock>(300, 300, nextStr));
	Common::SetActiveText(nextStr);
	std::cout << m_textBlockContainer.back()->m_textString->GetText() << std::endl;
}

std::string GameEngine::TextBlockManager::GetNextString()
{
	return std::string("Cat");
}
