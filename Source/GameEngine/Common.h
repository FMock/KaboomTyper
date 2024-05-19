#pragma once
#include <string>
#include <cmath>  // for std::log
#include "AABB.h"

/// <summary>
/// Common.h holds common types used by various different other types
/// </summary>

namespace GameEngine
{
	enum class MoveDirection
	{
		NONE, LEFT, RIGHT, UP, DOWN, MOVE_DIRECTION_COUNT
	};

	class Common
	{
	private:
		static std::string s_submittedText;
		static std::string s_activeText;
		static std::string s_currentWordCategory;

	public:
		static void SubmitText(std::string text);
		static void SetActiveText(std::string text);
		static bool AABBIntersect(AABB box1, AABB box2);
		static constexpr float GRAVITY = 9.81f;
		static constexpr float BASE_SIZE = 1.0f;
		static constexpr float LOG_BASE = 10.0f;
		static constexpr float FLOOR = 880.0F; // The y-position the TextBlocks cannot exceed.
		static constexpr float CEILING = 200.0F;
	};
}
