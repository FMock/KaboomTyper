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
