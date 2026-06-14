#include <iostream>
#include <box2d/box2d.h>
#include "TextBlock.h"
#include "AABB.h"
#include "DrawUtils.h"
#include "Utilities.h"
#include "Common.h"
#include "PhysicsWorld.h"

/**
*
* TextBlock class represents a colored block with text on it
*/
using namespace GameEngine::Utility;
using namespace GameEngine;
using namespace DrawUtilities;
using std::ostringstream;

// initialize static member
TextBlock* GameEngine::TextBlock::s_activeTextBlock = nullptr;

GameEngine::TextBlock::TextBlock() : Sprite(), InputObserver(), m_color(std::make_unique<Color>()), m_velocity(0.0f), m_angle(0.0f),
                                   m_adjustedTextblockWidth(0), m_collided(false), m_texture(0),m_scaleFactor(0), m_remove(false),
	                               m_prev_change_x(0), m_prev_change_y(0), m_isHit(false), m_isMoving(true), m_fontHeight(0),
								   m_canMoveHorizontal(false)
{
	if (s_activeTextBlock == nullptr)
	{
		m_active = true;
		s_activeTextBlock = this;
	}
	else
	{
		m_active = false;
	}
}

TextBlock::TextBlock(int x, int y, std::string str, Colors color) : Sprite(x, y, 0, 0), m_textString(new TextString(str, x, y)), m_color(std::make_unique<Color>())
{
	Initialize(x, y, str, color);
}

GameEngine::TextBlock::~TextBlock()
{
	if (m_active)
	{
		s_activeTextBlock = nullptr;
	}
}

void TextBlock::InitializeTextBlock(float x, float y, std::string str, Colors color)
{
	Initialize(x, y, str, color);
	SetPosition(x, y);
}

void TextBlock::Initialize(float x, float y, std::string str, Colors color)
{
	if(!m_textString)
		m_textString = std::make_unique<TextString>();

	m_textString->Initialize(str.c_str(), x, y);
	m_fontHeight = (float)m_textString->GetFontHeight();

	m_colorStr = Color::s_colorMap[color];
	m_collided = false;
	m_remove = false;
	m_isHit = false;
	m_moveDirection = MoveDirection::DOWN;
	m_velocity = 0.0f;
	m_box.x = x;
	m_box.y = y;
	m_scaleFactor = m_textString->GetFontWidth();

	int textBlockWidth = ScaleTextBlockWidth(m_textString->GetTextSize(), m_color->s_colorParameters.textureWidth);
	SetSize(textBlockWidth, m_color->s_colorParameters.textureHeight);

	m_box.setW(m_size.first);
	m_box.setH(m_fontHeight);

	m_texture = m_color->s_colorParameters.m_stringColorTextureColorMap[m_colorStr];
	m_isMoving = true;
	m_angle = 0.0f;
	m_canMoveHorizontal = false;

	if (s_activeTextBlock == nullptr)
	{
		m_active = true;
		s_activeTextBlock = this;
	}
	else
	{
		m_active = false;
	}
}


// Draw the textBlock to the screen
void TextBlock::Draw()
{
	// First, draw the colored blocks scaled to fit the textstring
	glDrawSpriteScaledRotated(m_texture,
		(int)m_position.first,
		(int)m_position.second,
		m_size.first,
		m_size.second,
		1.0f, // 1.0f because textblock width is already scaled
		m_fontHeight, m_angle);

	// Next, draw the text over the colored blocks
	m_textString->DrawText(1.0, m_angle);
}

void TextBlock::Update(float deltaTime)
{
	// Once handed off to Box2D the block's transform comes from SyncFromBody(),
	// so the custom fall code below is skipped entirely.
	if (m_physicsControlled)
		return;

	// move 'em down unless they already reached the bottom
	if (m_position.second < Common::FLOOR && m_isMoving)
	{
		m_velocity += Common::GRAVITY * deltaTime; // Update the velocity with gravity
		m_position.second += m_velocity * deltaTime; // Calculate the new position with the current velocity

		m_box.setX(abs(m_position.first));
		m_box.setY(abs(m_position.second));

		// move TextBlock down the screen (positive y-directioin)
		m_textString->SetVelocity(m_velocity);
		m_textString->Update(deltaTime); 
	}
	else
	{
		if (!m_isMoving)
		{
			m_isMoving = false;
			m_canMoveHorizontal = false;
		}
	}
}

float TextBlock::GetNormalizedSize(float textSize)
{
	return std::log(textSize + Common::BASE_SIZE) / std::log(Common::LOG_BASE);
}

void TextBlock::SetVelocity(float velocity)
{
	m_velocity = velocity;
}

float TextBlock::GetVelocity() const
{
	return m_velocity;
}

void TextBlock::SetBody(b2Body* body)
{
	m_body = body;
	m_physicsControlled = (body != nullptr);
	if (m_physicsControlled)
	{
		// Box2D now owns this block's motion; stop the custom fall state.
		m_isMoving = false;
		m_canMoveHorizontal = false;
	}
}

b2Body* TextBlock::GetBody() const
{
	return m_body;
}

bool TextBlock::IsPhysicsControlled() const
{
	return m_physicsControlled;
}

// Mirror the rigid body's transform back onto the block (and its TextString) so the
// colored body and the text stay glued together as the block settles or topples.
void TextBlock::SyncFromBody()
{
	if (!m_body)
		return;

	const b2Vec2 center = m_body->GetPosition();           // body center, in meters
	const float halfW = m_box.w * 0.5f;
	const float halfH = m_box.h * 0.5f;

	const float topLeftX = PhysicsWorld::ToPixels(center.x) - halfW;
	const float topLeftY = PhysicsWorld::ToPixels(center.y) - halfH;

	m_position.first = topLeftX;
	m_position.second = topLeftY;
	m_box.setX(static_cast<int>(topLeftX));
	m_box.setY(static_cast<int>(topLeftY));

	// Box2D angle is radians (CCW positive); the draw helpers take degrees.
	m_angle = m_body->GetAngle() * (180.0f / PI);

	m_textString->SetX(topLeftX);
	m_textString->SetY(topLeftY);
	m_textString->SetAngle(m_angle);
}

void TextBlock::Collision(Sprite &sprite)
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


/// <summary>
/// Sets the size of the TextBlock to the same size as the text size
/// </summary>
/// <param name="textSize">The number of characters in the textstring</param>
/// <param name="blockWidth">The width of a single textblock (default is 1 pixels)</param>
/// <returns></returns>
int GameEngine::TextBlock::ScaleTextBlockWidth(size_t textSize, int blockWidth)
{
	m_adjustedTextblockWidth = static_cast<int>((textSize * m_scaleFactor * blockWidth));

	SetSize(m_adjustedTextblockWidth, -1); // -1 leaves height unchanged

	return m_adjustedTextblockWidth;
}

void TextBlock::RespondToObserved(InputManager* InputMgr)
{
	//std::cout << "RespondToObserved called for TextBlock at address: " << this << std::endl;

	if (!InputMgr->m_kbPrevState[SDL_SCANCODE_RIGHT] && InputMgr->m_kbState[SDL_SCANCODE_RIGHT])
	{
		m_moveDirection = GameEngine::MoveDirection::RIGHT;

		if (m_active)
		{
			if (m_position.first + m_textBlockWidth + Common::HORIZONTAL_STEP < Common::EDGE_RIGHT)
			{
				m_textString->MoveHorizontal(Common::HORIZONTAL_STEP);
				m_position.first += Common::HORIZONTAL_STEP;
			}
			else
			{
				m_position.first = Common::EDGE_RIGHT - m_textBlockWidth;
				m_textString->SetX(Common::EDGE_RIGHT - m_textBlockWidth);
			}
		}
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_LEFT] && InputMgr->m_kbState[SDL_SCANCODE_LEFT])
	{
		m_moveDirection = GameEngine::MoveDirection::LEFT;

		if (m_active)
		{
			if (m_position.first - Common::HORIZONTAL_STEP > Common::EDGE_LEFT)
			{
				m_textString->MoveHorizontal(-Common::HORIZONTAL_STEP);
				m_position.first -= Common::HORIZONTAL_STEP;
			}
			else
			{
				m_position.first = Common::EDGE_LEFT;
				m_textString->SetX(Common::EDGE_LEFT);
			}
		}
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_UP] && InputMgr->m_kbState[SDL_SCANCODE_UP])
	{
		// Do nothing
	}
	else if (!InputMgr->m_kbPrevState[SDL_SCANCODE_DOWN] && InputMgr->m_kbState[SDL_SCANCODE_DOWN])
	{
		// Do nothing
	}
	else
	{
		// Do nothing
		m_moveDirection = GameEngine::MoveDirection::NONE;
	}

	//std::cout << "RespondToObserved completed for TextBlock at address: " << this << std::endl;
}

void TextBlock::SetCanMoveHorizontal(bool canMoveHorizontal)
{
	m_canMoveHorizontal = canMoveHorizontal;
}

bool GameEngine::TextBlock::IsActive() const
{
	return m_active;
}

void GameEngine::TextBlock::SetActiveState(bool isActive)
{
	m_active = isActive;
}

void GameEngine::TextBlock::SetTextBlockWidth(int x)
{
	m_textBlockWidth = x;
}

int GameEngine::TextBlock::GetTextBlockWidth() const
{
	return m_textBlockWidth;
}

void GameEngine::TextBlock::Activate()
{
	if (s_activeTextBlock != nullptr)
	{
		s_activeTextBlock->m_active = false;
	}

	m_active = true;
	s_activeTextBlock = this;
}

void TextBlock::SetMovingState(bool state)
{
	this->m_isMoving = state;
}

bool GameEngine::TextBlock::GetMovingState()
{
	return this->m_isMoving;
}
