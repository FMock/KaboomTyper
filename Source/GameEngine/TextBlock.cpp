#include <iostream>
#include "TextBlock.h"
#include "AABB.h"
#include "DrawUtils.h"
#include "Utilities.h"

/**
* File Name - TextBlock.cpp
* Author - Frank Mock
* 
* TextBlock class represents a colored block with text on it
*/
using namespace GameEngine::Utility;
using namespace GameEngine;
using namespace DrawUtilities;
using std::ostringstream;
typedef Game_Data GD;

enum Direction{LEFT, RIGHT, UP, DOWN };

// ****** RETHINK THIS CONSTRUCTOR - DOES w and h NEED TO BE PASSED IN WITH EACH OBJECT
// ******
TextBlock::TextBlock(float x, float y, int w, int h, std::string s, std::map<std::string, GLuint>& tMap)
	:Sprite(x, y, w, h), m_strToImageMap(tMap), m_textString(new TextString())
{
	//FontParameters fontParamters;
	//Utilities::ReadXmlFile("../../Config/FontParameters.xml", fontParamters); // TODO: DON'T USE HARD-CODED PATHS
	//TextStringFont font;
	//font.image = fontParamters.m_texture;
	//font.imageWidth = fontParamters.m_fontsheetWidth;
	//font.imageHeight = fontParamters.m_fontsheetHeight;
	//font.frameWidth = fontParamters.m_fontWidth;
	//font.frameHeight = fontParamters.m_fontHeight;
	m_textString->Initialize(s.c_str(), x + 8, y - 5); // TODO: PUT IN CONFIG FILE. 5 PIXEL TEXTSTRING POSITION ADJUSTMENT SO ITS CENTERED ON THE TEXTBLOCK

	LoadColorVector();

	srand(time(0));
	m_color = m_colors.at(rand() % 7); // chooses a random color for this textblock

	//m_fontWidth = 31; // 23 previously - TODO THIS SHOULD NOT BE HARD-CODED
	//m_fontHeight = 36; //25 previously - TODO THIS SHOULD NOT BE HARD-CODED

	m_text = s;
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moving = true;
	m_direction = DOWN;
	box.setW(m_size.first);
	box.setH(m_size.second);

	setHeight(h); // 30 a single textblock is 30 pixels high, When constructed we only need the height

	m_textSize = m_text.size(); // TODO: THERE SHOULD BE A m_textSize member of TextString that TextBlock uses
	ScaleTextBlock(m_textSize, TextBlockParameters::defaultBlockWidth);
}

TextBlock::TextBlock(int x, int y, TextBlockParameters& textBlockParams, std::string str, std::map<std::string, GLuint>& tMap) 
	: Sprite(x, y, 0, 0), m_strToImageMap(tMap), m_textString(new TextString())
{
	m_textString->Initialize(str.c_str(), x + 8, y - 5); // TODO: PUT IN CONFIG FILE. 5 PIXEL TEXTSTRING POSITION ADJUSTMENT SO ITS CENTERED ON THE TEXTBLOCK

	LoadColorVector();

	srand(time(0));
	m_color = m_colors.at(rand() % 7);
	m_text = str;
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moving = true;
	m_direction = DOWN;
	box.setW(m_size.first);
	box.setH(m_size.second);

	setHeight(textBlockParams.blockHeight); // 30 a single textblock is 30 pixels high, When constructed we only need the height

	// Adjust the number of blocks that make up this textblock, so the TextString and TextBlock are similar width
	ScaleTextBlock(m_textSize, textBlockParams.blockWidth);
}

GameEngine::TextBlock::~TextBlock()
{
	delete m_textString;
}

void TextBlock::InitializeTextBlock(float x, float y, TextBlockParameters& textBlockParams, std::string str, const std::map<std::string, GLuint>& tMap)
{
	m_strToImageMap = tMap;
	m_textString = new TextString();
	m_textString->Initialize(str.c_str(), x + 8, y - 5); // TODO: PUT IN CONFIG FILE. 5 PIXEL TEXTSTRING POSITION ADJUSTMENT SO ITS CENTERED ON THE TEXTBLOCK

	LoadColorVector();

	srand(time(0));
	m_color = m_colors.at(rand() % 7);
	m_text = str;
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moving = true;
	m_direction = DOWN;
	box.setW(m_size.first);
	box.setH(m_size.second);

	setHeight(textBlockParams.blockHeight); // 30 a single textblock is 30 pixels high, When constructed we only need the height

	// Adjust the number of blocks that make up this textblock, so the TextString and TextBlock are similar width
	ScaleTextBlock(m_textSize, textBlockParams.blockWidth);
}

// Draw the textBlock to the screen
void TextBlock::Draw()
{
	// First, draw the colored blocks
	glDrawSprite(m_strToImageMap[m_color], (int) m_position.first, (int)m_position.second, m_size.first, m_size.second);

	// Next, draw the text over the colored blocks
	m_textString->DrawText();
}

void TextBlock::Update(float deltaTime)
{
	if(m_moving){
		moveDown();

		if(m_position.first > GD::WORLD_WIDTH - m_size.first - GD::BLOCK_AREA_TO_RH_EDGE){
			change_x = 0; //stop block from going out of bounds (right boundry)
			m_position.first -= 1;
		}

		if(m_position.first < 0){
			change_x = 0; //stop block from going out of bounds (left boundry)
			m_position.first += 1;
		}

		if(m_position.second < 0){
			change_y = 0; //stop block from going out of bounds up (upper boundry)
			m_position.second += 1;
		}

		if(m_position.second > GD::WORLD_HEIGHT - m_size.second - GD::BL_FLOOR_TO_BOTTOM){
			change_y = 0; //stop block from going out of bounds up (lower boundry)
			m_position.second -= 1;
			m_moving = false;
		}

		m_position.first += change_x * deltaTime;
		box.setX(abs(m_position.first));
		m_position.second += change_y * deltaTime;
		box.setY(abs(m_position.second));
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

void GameEngine::TextBlock::RespondToObserved(InputManager* InputMgr)
{
	if (!InputMgr->m_kbPrevState[SDL_SCANCODE_RIGHT] && InputMgr->m_kbState[SDL_SCANCODE_RIGHT])
	{
		m_moveDirection = GameEngine::MoveDirection::RIGHT;
		std::cout << "TextBlock move right" << std::endl;
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_LEFT] && InputMgr->m_kbState[SDL_SCANCODE_LEFT])
	{
		m_moveDirection = GameEngine::MoveDirection::LEFT;
		std::cout << "TextBlock move left" << std::endl;
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_UP] && InputMgr->m_kbState[SDL_SCANCODE_UP])
	{
		m_moveDirection = GameEngine::MoveDirection::UP;
		std::cout << "TextBlock move up not allowed!" << std::endl;
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_DOWN] && InputMgr->m_kbState[SDL_SCANCODE_DOWN])
	{
		m_moveDirection = GameEngine::MoveDirection::DOWN;
		std::cout << "TextBlock move down" << std::endl;
	}
	else
	{
		m_moveDirection = GameEngine::MoveDirection::NONE;
	}
}

void TextBlock::SetActiveState(bool state)
{
	this->m_isActive = state;
}

bool GameEngine::TextBlock::GetActiveState()
{
	return this->m_isActive;
}
