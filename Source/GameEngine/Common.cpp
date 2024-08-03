#include "Common.h"
#include "GlobalPreprocessorFlags.h"
#include <iostream>

using namespace GameEngine;

float Common::GRAVITY = 9.81f;
const std::map<std::string, float> Common::GRAVITY_CHOICES = {
	{"Default", 18.00f},
	{"Turtle", 9.81f},
	{"Hare", 26.00f},
	{"Cheetah", 36.0f}
};
std::string Common::s_submittedText = "";
std::string Common::s_activeText = "";
std::string Common::s_currentWordCategory = "";
Colors Common::s_currentColor = DEFAULT_COLOR;
Colors Common::s_previousColor = DEFAULT_COLOR;
std::pair<float, float> Common::s_currentPosition = { 0.0f, 0.0f };
int Common::s_currentTextBlockWidth = 0;
GameState Common::CurrentState = GameState::STOPPED;
const std::string Common::DEFAULT_WORD_CATEGORY = "Default";

void Common::SubmitText(std::string text)
{
	s_submittedText = text;

#if DEBUG
	std::cout << s_submittedText << std::endl;
#endif
}

std::string GameEngine::Common::GetSubmittedText()
{
	return s_submittedText;
}

void Common::SetActiveText(std::string text)
{
	s_activeText = text;
}

std::string Common::GetActiveText()
{
	return s_activeText;
}

/*
* Checks AABB/AABB collisions
* Used to determine if a TextBlock has touched another TextBlock
*/
bool Common::AABBIntersect(AABB box1, AABB box2)
{
	//box1 to the right
	if (box1.x > box2.x + box2.w)
	{
		return false;
	}

	// box1 top the left
	if (box1.x + box1.w < box2.x)
	{
		return false;
	}

	// box1 below
	if (box1.y > box2.y + box2.h)
	{
		return false;
	}

	// box1 above
	if (box1.y + box1.h < box2.y)
	{
		return false;
	}

	return true;
}
