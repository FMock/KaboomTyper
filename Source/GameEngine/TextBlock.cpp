#include "TextBlock.h"
#include "AABB.h"
#include "DrawUtils.h"
#include "Utilities.h"

using namespace GameEngine::Utility;

/**
* File Name - TextBlock.cpp
* Author - Frank Mock
* 
* TextBlock class represents a colored block with text on it
*/
using namespace GameEngine;
using namespace DrawUtilities;
using std::ostringstream;
typedef Game_Data GD;

enum Direction{LEFT, RIGHT, UP, DOWN };

// ****** RETHINK THIS CONSTRUCTOR - DOES w and h NEED TO BE PASSED IN WITH EACH OBJECT
// ******
TextBlock::TextBlock(int x, int y, int w, int h, std::string s, std::map<std::string, GLuint>& tMap)
	:Sprite(x, y, w, h), m_strToImageMap(tMap), m_textString(new TextString())
{
	FontParameters fontParamters;
	Utilities::ReadXmlFile("../../Config/FontParameters.xml", fontParamters); // TODO: DON'T USE HARD-CODED PATHS
	TextStringFont font;
	font.image = fontParamters.m_texture;
	font.imageWidth = fontParamters.m_fontsheetWidth;
	font.imageHeight = fontParamters.m_fontsheetHeight;
	font.frameWidth = fontParamters.m_fontWidth;
	font.frameHeight = fontParamters.m_fontHeight;
	m_textString->Initialize(s.c_str(), x + 8, y - 5, font); // TODO: PUT IN CONFIG FILE. 5 PIXEL TEXTSTRING POSITION ADJUSTMENT SO ITS CENTERED ON THE TEXTBLOCK

	LoadColorVector();

	srand(time(0));
	m_color = m_colors.at(rand() % 7); // chooses a random color for this textblock

	m_fontWidth = 31; // 23 previously - TODO THIS SHOULD NOT BE HARD-CODED
	m_fontHeight = 36; //25 previously - TODO THIS SHOULD NOT BE HARD-CODED

	m_text = s;
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moving = true;
	m_direction = DOWN;
	box.setW(m_characterSize[0]);
	box.setH(m_characterSize[1]);

	setHeight(h); // 30 a single textblock is 30 pixels high, When constructed we only need the height

	m_textSize = m_text.size(); // TODO: THERE SHOULD BE A m_textSize member of TextString that TextBlock uses
	ScaleTextBlock(m_textSize, TextBlockParameters::defaultBlockWidth);
}

TextBlock::TextBlock(int x, int y, TextBlockParameters& textBlockParams, TextStringFont& font, std::string str, std::map<std::string, GLuint>& tMap) 
	: Sprite(x, y, textBlockParams.blockWidth, textBlockParams.blockHeight), m_strToImageMap(tMap), m_textString(new TextString())
{
	m_textString->Initialize(str.c_str(), x + 8, y - 5, font); // TODO: PUT IN CONFIG FILE. 5 PIXEL TEXTSTRING POSITION ADJUSTMENT SO ITS CENTERED ON THE TEXTBLOCK

	LoadColorVector();

	srand(time(0));
	m_color = m_colors.at(rand() % 7);
	m_text = str;
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moving = true;
	m_direction = DOWN;
	box.setW(m_characterSize[0]);
	box.setH(m_characterSize[1]);

	setHeight(textBlockParams.blockHeight); // 30 a single textblock is 30 pixels high, When constructed we only need the height

	// Adjust the number of blocks that make up this textblock, so the TextString and TextBlock are similar width
	ScaleTextBlock(m_textSize, textBlockParams.blockWidth);
}

GameEngine::TextBlock::~TextBlock()
{
	delete m_textString;
}


// Draw the textBlock to the screen
void TextBlock::Draw()
{
	// First, draw the colored blocks
	glDrawSprite(m_strToImageMap[m_color],  m_characterPos[0], m_characterPos[1], m_characterSize[0], m_characterSize[1]);

	// Next, draw the text over the colored blocks
	m_textString->DrawText();
}

void TextBlock::update(float deltaTime)
{
	if(m_moving){
		moveDown();

		if(m_characterPos[0] > GD::WORLD_WIDTH - m_characterSize[0] - GD::BLOCK_AREA_TO_RH_EDGE){
			change_x = 0; //stop block from going out of bounds (right boundry)
			m_characterPos[0] -= 1;
		}

		if(m_characterPos[0] < 0){
			change_x = 0; //stop block from going out of bounds (left boundry)
			m_characterPos[0] += 1;
		}

		if(m_characterPos[1] < 0){
			change_y = 0; //stop block from going out of bounds up (upper boundry)
			m_characterPos[1] += 1;
		}

		if(m_characterPos[1] > GD::WORLD_HEIGHT - m_characterSize[1] - GD::BL_FLOOR_TO_BOTTOM){
			change_y = 0; //stop block from going out of bounds up (lower boundry)
			m_characterPos[1] -= 1;
			m_moving = false;
		}

		m_characterPos[0] += change_x * deltaTime;
		box.setX(abs(m_characterPos[0]));
		m_characterPos[1] += change_y * deltaTime;
		box.setY(abs(m_characterPos[1]));
	}
	else
	{
		stop();
	}
}

void TextBlock::moveLeft()
{
	m_moving = true;
	m_prev_change_y = change_y;
	change_y = 0;
	m_prev_change_x = change_x;
	//change_x = -speedX;
	m_direction = LEFT;
}

void TextBlock::moveRight()
{
	m_moving = true;
	m_prev_change_y = change_y;
	change_y = 0;
	m_prev_change_x = change_x;
	//change_x = speedX;
	m_direction = RIGHT;
}

void TextBlock::moveUp()
{
	m_moving = true;
	m_prev_change_x = change_x;
	change_x = 0;
	m_prev_change_y = change_y;
	//change_y = -speedY;
	m_direction = UP;
}

void TextBlock::moveDown()
{
	m_moving = true;
	m_prev_change_x = change_x;
	change_x = 0;
	m_prev_change_y = change_y;
	//change_y = speedY;
	m_direction = DOWN;
}

void TextBlock::stop()
{
	m_moving = false;
	m_prev_change_x = change_x;
	change_x = 0;
	m_prev_change_y = change_y;
	change_y = 0;
}

void TextBlock::collision(Sprite &sprite)
{
	m_collided = true;
}


std::string TextBlock::to_string() const
{
	ostringstream oss;
	oss << "TextBlock ************\n"
		<< "isDead = " << remove << "\n"
		<< "moving = " << m_moving << "\n"
		<< "Direction = " << m_direction << "\n"
		<< "isHit = "  << m_isHit << "\n"
		<< "AABB x = " << box.x << "\n"
		<< "AABB y = " << box.y << "\n"
		<< "AABB w = " << box.w << "\n"
		<< "AABB h = " << box.h << "\n"
		<< "xPos = " << getXPos() << "\n"
		<< "YPos = " << getYPos() << "\n"
		<< "END TextBlock *********\n";
	return oss.str();
}

void GameEngine::TextBlock::LoadColorVector()
{
	m_colors.push_back("white"), m_colors.push_back("blue"), m_colors.push_back("green");
	m_colors.push_back("yellow"), m_colors.push_back("purple"), m_colors.push_back("red");
	m_colors.push_back("orange");
}

void GameEngine::TextBlock::ScaleTextBlock(int textSize, int blockWidth)
{
	m_scaleFactor = 0.60f; // TODO PUT THIS IN CONFIG FILE
	m_adjustedTextblockWidth = static_cast<int>((m_text.size() * blockWidth * m_scaleFactor));
	setWidth(m_adjustedTextblockWidth);
}
