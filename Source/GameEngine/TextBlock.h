#pragma once

#include <GL/glew.h>
#include <map>
#include <vector>
#include <sstream>
#include <memory>
#include <ctime>
#include "game_data.h"
#include "sprite.h"
#include "TextString.h"
#include "TextBlockParameters.h"
#include "TextStringFont.h"
#include "InputObserver.h"
#include "InputManager.h"

/// <summary>
/// A TextBlock is a composite of colored blocks with a TextString 
/// </summary>

namespace GameEngine
{
	class TextBlock : public Sprite, public InputObserver
	{
	public:
		TextBlock();
		TextBlock(int x, int y, std::string);
		~TextBlock();
		void InitializeTextBlock(float x, float y, std::string str);

		std::unique_ptr<TextString> m_textString; // TextString that's drawn over the TextBlock
		std::vector<std::string> m_colors;
		std::string m_color;

		int m_prev_change_x;
		int m_prev_change_y;

		bool m_collided;
		bool m_isHit; // True when hit by any enemy
		bool m_remove; // If True remove TextBlock from game

		void Update(float deltaTime) override;
		void Draw() override;
		void collision(Sprite& sprite);
		std::string to_string() const;
		void SetActiveState(bool state);
		bool GetActiveState();

		// Static members to share across all instances
		static TextBlockParameters s_textBlockParameters;
		static bool s_textBlockInitialized;

	private:
		float m_scaleFactor;
		int m_adjustedTextblockWidth;
		bool InitializeTextBlockParameters();
		void LoadColorVector();
		int ScaleTextBlockWidth(int textSize, int blockWidth);
		bool m_isActive;

	protected:
		void RespondToObserved(InputManager* InputMgr) override;
	};
}
