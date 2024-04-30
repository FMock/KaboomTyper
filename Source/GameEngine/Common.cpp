#include "Common.h"
#include <iostream>

using namespace GameEngine;

std::string Common::s_submittedText = "";

void Common::SubmitText(std::string text)
{
	s_submittedText = text;

	std::cout << s_submittedText << std::endl;
}