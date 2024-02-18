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
		//TextBlock(int, int, TextBlockParameters&, TextString&, std::string, std::map<std::string, GLuint>& tMap);
		~TextBlock();
		TextString* m_textString;

		std::map<std::string, GLuint> m_strToImageMap;

		std::vector<std::string> m_colors;

		std::string m_color;

		std::string m_text; // the text to be displayed on the block

		int fontWidth;
		int fontHeight;

		// The direction the character is facing
		int direction;

		int prev_change_x;

		int prev_change_y;

		// If x and y velocity is not 0
		bool moving;

		bool collided;

		// True when hit by any enemy
		bool isHit;

		// If True remove TextBlock from game
		bool remove;

		void update(float deltaTime);

		void Draw();

		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void stop();
		void collision(Sprite& sprite);
		std::string to_string() const;
	};
}

#endif