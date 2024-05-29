#pragma once
#include <string>
#include <cmath>  // for std::log
#include "AABB.h"
#include "Color.h"

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
		static std::string GetSubmittedText();
		static void SetActiveText(std::string text);
		static std::string GetActiveText();
		static bool AABBIntersect(AABB box1, AABB box2);
		static constexpr float GRAVITY = 9.81f;
		static constexpr float BASE_SIZE = 1.0f;
		static constexpr float LOG_BASE = 10.0f;
		static constexpr float FLOOR = 878.0F; // The y-position the TextBlocks cannot exceed.
		static constexpr float CEILING = 200.0F;
		static constexpr float EDGE_LEFT = 10.0F;
		static constexpr float EDGE_RIGHT = 790.0F;
		static constexpr float HORIZONTAL_STEP = 24.0F;
		static Colors s_currentColor;
		static Colors s_previousColor;
		static std::pair<float, float> s_currentPosition;
		static int s_currentTextBlockWidth;
	};
}
