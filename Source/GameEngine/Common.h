#pragma once
#include <string>
#include <cmath>  // for std::log
#include "AABB.h"
#include "Color.h"
#include "GameStates.h"
#include <map>

#define PI 3.14159265f

/// <summary>
/// Common.h holds common types used by various different other types
/// </summary>

namespace GameEngine
{
	enum class MoveDirection
	{
		NONE, LEFT, RIGHT, UP, DOWN, MOVE_DIRECTION_COUNT
	};

	// Game difficulty: controls how many word blocks may fall at once.
	// Easy = one at a time, Normal = up to two, Hard = up to three.
	enum class Difficulty
	{
		Easy, Normal, Hard
	};

	// Display/lookup helpers for Difficulty (kept inline in the header so any TU can use them).
	inline std::string DifficultyName(Difficulty d)
	{
		switch (d)
		{
		case Difficulty::Easy: return "Easy";
		case Difficulty::Hard: return "Hard";
		case Difficulty::Normal:
		default:               return "Normal";
		}
	}

	inline Difficulty ParseDifficulty(const std::string& name)
	{
		if (name == "Easy") return Difficulty::Easy;
		if (name == "Hard") return Difficulty::Hard;
		return Difficulty::Normal; // default
	}

	class Common
	{
	private:
		static std::string s_submittedText;
		static std::string s_activeText;
		static std::string s_currentWordCategory;

	public:
		static const std::string DEFAULT_WORD_CATEGORY;
		static const int WINDOW_WIDTH = 900; // logical width; widened from 800 so the Word Category fly-out fits (matches the 900px menu bar)
		static const int WINDOW_HEIGHT = 976; // logical height; trimmed so the bordered window (976 + 2*border) fits comfortably under the taskbar
		static const int BORDER_THICKNESS = 4; // red border drawn around the whole window (window is enlarged by 2x this)
		static const int FRAME_THICKNESS = 3;  // thickness of the red frames on menus, buttons, message boxes
		static void SubmitText(std::string text);
		static std::string GetSubmittedText();
		static void SetActiveText(std::string text);
		static std::string GetActiveText();
		static bool AABBIntersect(AABB box1, AABB box2);
		static float GRAVITY;
		static constexpr float BASE_SIZE = 1.0f;
		static constexpr float LOG_BASE = 10.0f;
		static constexpr int FLOOR = 878; // The y-position the TextBlocks cannot exceed.
		static constexpr int CEILING = 200;
		static constexpr int EDGE_LEFT = 10;
		static constexpr int EDGE_RIGHT = 889; // tracks the right side bar inner edge (window widened to 900)
		static constexpr float HORIZONTAL_STEP = 24.0F; //24.0
		static constexpr int FONT_HEIGHT = 34;
		static constexpr int TEXTBLOCK_VERTICAL_START_POSITION = 115;
		static Colors s_currentColor;
		static Colors s_previousColor;
		static std::pair<float, float> s_currentPosition;
		static int s_currentTextBlockWidth;
		static constexpr int BUTTON_POSTION_OFFSET = 4;
		static constexpr int MENUITEM_POSTION_OFFSET = 4;
		inline static const std::string GAME_CONFIG_FILE = R"(..\..\Config\GameEngineConfig.txt)"; // must be inline for C++17 and later to define in header
		static GameState CurrentState;
		static const std::map<std::string, float> GRAVITY_CHOICES;
	};
}
