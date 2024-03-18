#pragma once
#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "sprite.h"
#include <GL/glew.h>
#include <map>
#include <vector>
#include "game_data.h"
#include <sstream>
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
		TextBlock(int, int, int, int, std::string, std::map<std::string, GLuint>& tMap);
		TextBlock(int, int, TextBlockParameters&, TextStringFont&, std::string, std::map<std::string, GLuint>& tMap);
		~TextBlock();

		std::string m_text;        // the text for the TextString
		int m_textSize;
		TextString* m_textString; // TextString that's drawn over the TextBlock
		std::map<std::string, GLuint> m_strToImageMap;
		std::vector<std::string> m_colors;
		std::string m_color;

		int m_fontWidth;
		int m_fontHeight;

		// The direction the textblock is moving
		int m_direction;
		int m_prev_change_x;
		int m_prev_change_y;

		// If x and y velocity is not 0
		bool m_moving;

		bool m_collided;

		// True when hit by any enemy
		bool m_isHit;

		// If True remove TextBlock from game
		bool m_remove;

		void Update(float deltaTime) override;

		void Draw();

		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void stop();
		void collision(Sprite& sprite);
		std::string to_string() const;
		void SetActiveState(bool state);
		bool GetActiveState();

	private:
		float m_scaleFactor;
		int m_adjustedTextblockWidth;
		void LoadColorVector();
		void ScaleTextBlock(int textSize, int blockWidth);
		bool m_isActive;

	protected:
		virtual void RespondToObserved(InputManager* InputMgr);
	};
}

#endif