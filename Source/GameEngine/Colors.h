#pragma once

#include <random>

namespace GameEngine
{
	enum Colors
	{
		RED = 0,
		DARK_RED,
		GREEN,
		DARK_GREEN,
		BLUE,
		DARK_BLUE,
		PURPLE,
		DARK_PURPLE,
		YELLOW,
		DARK_YELLOW,
		ORANGE,
		BROWN,
		GRAY,
		DARK_GRAY,
		WHITE,
		DEFAULT_COLOR = WHITE,
		BLACK,
		UNKNOWN,
		NONE
	};

	inline static Colors GetRandomColor()
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());

		// Adjust range to exclude BLACK, UNKNOWN, and NONE
		static std::uniform_int_distribution<int> dist(0, WHITE);

		Colors color;
		do
		{
			color = static_cast<Colors>(dist(gen));
		} while (color == BLACK || color == UNKNOWN || color == NONE);

		return color;
	}
}
