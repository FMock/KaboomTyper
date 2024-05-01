#include "TextBlockGenerator.h"
#include <iostream>

using namespace GameEngine;

TextBlockGenerator::TextBlockGenerator() : m_textBlockContainer()
{
}

TextBlockGenerator::~TextBlockGenerator()
{
}

void TextBlockGenerator::NextTextBlock()
{
	std::string nextStr = GetNextString();
	m_textBlockContainer.push_back(std::make_unique<TextBlock>(300, 300, nextStr));
	Common::SetActiveText(nextStr);
	std::cout << m_textBlockContainer.back()->m_textString->GetText() << std::endl;
}

std::string GameEngine::TextBlockGenerator::GetNextString()
{
	return std::string("Cat");
}
