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

// Definition of static members
TextBlockParameters TextBlock::s_textBlockParameters;
bool TextBlock::s_textBlockInitialized;

GameEngine::TextBlock::TextBlock() : Sprite(), InputObserver()
{

}

TextBlock::TextBlock(int x, int y, std::string str) : Sprite(x, y, 0, 0), m_textString(new TextString(str, x, y))
{
	m_textString->Initialize(str.c_str(), x, y);

	LoadColorVector();

	srand(time(0));
	m_color = m_colors.at(rand() % 7);
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moveDirection = MoveDirection::DOWN;
	m_box.setW(m_size.first);
	m_box.setH(m_size.second);

	if (!s_textBlockInitialized)
	{
		InitializeTextBlockParameters();
	}

	m_scaleFactor = m_textString->GetFontWidth();
	int textBlockWidth = ScaleTextBlockWidth(m_textString->GetTextSize(), s_textBlockParameters.blockWidth);
	setSize(textBlockWidth, s_textBlockParameters.blockHeight);
}

GameEngine::TextBlock::~TextBlock()
{
}

void TextBlock::InitializeTextBlock(float x, float y, std::string str)
{
	m_textString = std::make_unique<TextString>();
	m_textString->Initialize(str.c_str(), x, y);

	LoadColorVector();

	srand(time(0));
	m_color = m_colors.at(rand() % 7);
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moveDirection = MoveDirection::DOWN;
	m_box.setW(m_size.first);
	m_box.setH(m_size.second);

	if (!s_textBlockInitialized)
	{
		InitializeTextBlockParameters();
	}
	m_scaleFactor = m_textString->GetFontWidth();
	setPosition(x, y);
	int textBlockWidth = ScaleTextBlockWidth(m_textString->GetTextSize(), s_textBlockParameters.blockWidth);
	setSize(textBlockWidth, s_textBlockParameters.blockHeight);
}

bool GameEngine::TextBlock::InitializeTextBlockParameters()
{
	bool initialized = Utilities::ReadXmlFile("../../Config/TextBlockParameters.xml", s_textBlockParameters); // TODO: DON'T USE HARD-CODED PATHS

	if (!initialized)
	{
		throw std::runtime_error("Failed to initialize TextBlockParameters from XML file.");
	}

	// Load all the textures into the String-to-Image map
	s_textBlockParameters.m_stringColorTextureColorMap["red"] = s_textBlockParameters.redBlockTexture;
	s_textBlockParameters.m_stringColorTextureColorMap["blue"] = s_textBlockParameters.blueBlockTexture;
	s_textBlockParameters.m_stringColorTextureColorMap["green"] = s_textBlockParameters.greenBlockTexture;
	s_textBlockParameters.m_stringColorTextureColorMap["yellow"] = s_textBlockParameters.yellowBlockTexture;
	s_textBlockParameters.m_stringColorTextureColorMap["purple"] = s_textBlockParameters.purpleBlockTexture;
	s_textBlockParameters.m_stringColorTextureColorMap["white"] = s_textBlockParameters.whiteBlockTexture;
	s_textBlockParameters.m_stringColorTextureColorMap["orange"] = s_textBlockParameters.orangeBlockTexture;

	s_textBlockInitialized = true;
}

// Draw the textBlock to the screen
void TextBlock::Draw()
{
	// First, draw the colored blocks scaled to fit the textstring
	glDrawSpriteScaled(s_textBlockParameters.m_stringColorTextureColorMap[m_color], 
		              (int) m_position.first, (int)m_position.second, m_size.first, 
		              m_size.second, 1.0f, 32.0f); // 1.0 because textblock width is already scaled, 32.0 to scale to same height as font

	// Next, draw the text over the colored blocks
	m_textString->DrawText();
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
	m_colors.push_back("white"), m_colors.push_back("blue"), m_colors.push_back("green");
	m_colors.push_back("yellow"), m_colors.push_back("purple"), m_colors.push_back("red");
	m_colors.push_back("orange");
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
