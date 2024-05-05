#include <iostream>
#include "TextBlock.h"
#include "AABB.h"
#include "DrawUtils.h"
#include "Utilities.h"

/**
*
* TextBlock class represents a colored block with text on it
*/
using namespace GameEngine::Utility;
using namespace GameEngine;
using namespace DrawUtilities;
using std::ostringstream;
typedef Game_Data GD;

GameEngine::TextBlock::TextBlock() : Sprite(), InputObserver(), m_color(std::make_unique<Color>()),
                                   m_adjustedTextblockWidth(0), m_collided(false), m_texture(0),m_scaleFactor(0), m_remove(false),
	                               m_prev_change_x(0), m_prev_change_y(0), m_isHit(false), m_isActive(true), m_fontHeight(0)
{
}

TextBlock::TextBlock(int x, int y, std::string str, Colors color) : Sprite(x, y, 0, 0), m_textString(new TextString(str, x, y)), m_color(std::make_unique<Color>())
{
	Initialize(x, y, str, color);
}

GameEngine::TextBlock::~TextBlock()
{
}

void TextBlock::InitializeTextBlock(float x, float y, std::string str, Colors color)
{
	Initialize(x, y, str, color);
	setPosition(x, y);
}

void TextBlock::Initialize(float x, float y, std::string str, Colors color)
{
	if(!m_textString)
		m_textString = std::make_unique<TextString>();

	m_textString->Initialize(str.c_str(), x, y);
	m_fontHeight = (float)m_textString->GetFontHeight();

	LoadColorVector(); // load strings of color names
	//short numberOfColors = m_color->s_colorParameters.numberOfColors;
	//srand(time(0));
	//m_colorStr = m_colors.at(rand() % numberOfColors - 1); // minus one because we don't want any black textblocks
	m_colorStr = m_colors.at((int)color); // minus one because we don't want any black textblocks
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moveDirection = MoveDirection::DOWN;
	m_box.setW(m_size.first);
	m_box.setH(m_size.second);
	m_scaleFactor = m_textString->GetFontWidth();

	int textBlockWidth = ScaleTextBlockWidth(m_textString->GetTextSize(), m_color->s_colorParameters.textureWidth);
	setSize(textBlockWidth, m_color->s_colorParameters.textureHeight);

	m_texture = m_color->s_colorParameters.m_stringColorTextureColorMap[m_colorStr];
}


// Draw the textBlock to the screen
void TextBlock::Draw()
{
	// First, draw the colored blocks scaled to fit the textstring
	glDrawSpriteScaled(m_texture,
		(int)m_position.first,
		(int)m_position.second,
		m_size.first,
		m_size.second,
		1.0f, // 1.0f because textblock width is already scaled
		m_fontHeight);

	// Next, draw the text over the colored blocks
	m_textString->DrawText(1.0);
}

void TextBlock::Update(float deltaTime)
{
	// TODO: ADD UPDATE CODE
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
		<< "Direction = " << (int)m_moveDirection << "\n"
		<< "isHit = "  << m_isHit << "\n"
		<< "AABB x = " << m_box.x << "\n"
		<< "AABB y = " << m_box.y << "\n"
		<< "AABB w = " << m_box.w << "\n"
		<< "AABB h = " << m_box.h << "\n"
		//<< "xPos = " << getXPos() << "\n"
		//<< "YPos = " << getYPos() << "\n"
		<< "END TextBlock *********\n";
	return oss.str();
}

void GameEngine::TextBlock::LoadColorVector()
{
	m_colors.push_back("red"), // 0
	m_colors.push_back("darkRed"),  // 1
	m_colors.push_back("green"); // 2

	m_colors.push_back("darkGreen"), // 3
	m_colors.push_back("blue"), // 4
	m_colors.push_back("darkBlue"); // 5

	m_colors.push_back("purple"), // 6
	m_colors.push_back("Darkpurple"), // 7
	m_colors.push_back("yellow"); // 8

	m_colors.push_back("darkYellow"), // 9
	m_colors.push_back("orange"), // 10
	m_colors.push_back("brown"); // 11

	m_colors.push_back("gray"), // 12
	m_colors.push_back("darkGray"), // 13
	m_colors.push_back("white"); // 14
	m_colors.push_back("black"); // 15
}

/// <summary>
/// Sets the size of the TextBlock to the same size as the text size
/// </summary>
/// <param name="textSize">The number of characters in the textstring</param>
/// <param name="blockWidth">The width of a single textblock (default is 1 pixels)</param>
/// <returns></returns>
int GameEngine::TextBlock::ScaleTextBlockWidth(int textSize, int blockWidth)
{
	m_adjustedTextblockWidth = static_cast<int>((textSize * m_scaleFactor * blockWidth));

	setSize(m_adjustedTextblockWidth, -1); // -1 leaves height unchanged

	return m_adjustedTextblockWidth;
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
