#pragma once
#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "sprite.h"
#include <GL/glew.h>
#include <map>
#include <vector>
#include "game_data.h"
#include <sstream>
#include <memory>
#include <ctime>
#include "TextString.h"
#include "TextBlockParameters.h"
#include "TextStringFont.h"

// Author - Frank Mock
// TextBlock.h
// TextBlock is a composite sprite consisting of a rectangular 
// block with text imposed over it

namespace GameEngine
{
	class TextBlock : public Sprite
	{
	public:
		TextBlock() = default;
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
		void Draw();
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
		virtual void RespondToObserved(InputManager* InputMgr);
	};
}

#endif