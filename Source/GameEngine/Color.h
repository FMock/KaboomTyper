#pragma once

#include "ColorParameters.h"

namespace GameEngine
{
	class Color
	{
	public:
		Color();
		~Color();

		// Static members to share across all instances
		static ColorParameters s_colorParameters;
		static bool s_initialized;

	private:
		bool InitializeColorParameters();
	};
}