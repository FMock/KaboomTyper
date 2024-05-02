#include "Common.h"
#include <iostream>

using namespace GameEngine;

std::string Common::s_submittedText = "";
std::string Common::s_activeText = "";
std::string Common::s_currentWordCategory = "";

void Common::SubmitText(std::string text)
{
	s_submittedText = text;
}

void Common::SetActiveText(std::string text)
{
	s_activeText = text;
}
