#include "Button.h"

using namespace GameEngine;


Button::Button() : m_outline(std::make_unique<RectangleDrawable>()), m_label(std::make_unique<TextString>()), 
                   m_adjustedWidth(0), m_scaler(1.0f), m_xPos(0), m_yPos(0), m_width(0), m_height(0)
{

}

Button::Button(std::string text, int x, int y, float scaler, Colors color) : m_outline(std::make_unique<RectangleDrawable>()), m_adjustedWidth(0)
{
	Initialize(text, x, y, scaler, color);
}

Button::~Button()
{
}

void Button::Initialize(std::string text, int x, int y, float scaler, Colors color)
{
	m_xPos = x - Common::BUTTON_POSTION_OFFSET;
	m_yPos = y - Common::BUTTON_POSTION_OFFSET;
	m_scaler = scaler;
	m_adjustedWidth = static_cast<int>((text.size() * 24 * 1 * m_scaler)); // 24 is font texture width
	m_width = m_adjustedWidth + (2 * Common::BUTTON_POSTION_OFFSET);
	m_height = 32 * m_scaler + (2 * Common::BUTTON_POSTION_OFFSET); // 32 is font texture height
	m_outline->Initialize(m_xPos, m_yPos, m_adjustedWidth + 8, 32 * m_scaler + 8, Colors::DEFAULT_COLOR);
	m_label->Initialize(text, x, y);
}

void Button::Update(float dt)
{
}

void Button::Draw()
{
	m_outline->Draw();
	m_label->DrawText(m_scaler);
}

void Button::ButtonPressed()
{
}

bool Button::IsMouseOverButton(const int& x, const int& y)
{
	return (x >= m_xPos && x <= m_xPos + m_width &&
		y >= m_yPos && y <= m_yPos + m_height);
}

void Button::ButtonReleased()
{
}

void Button::SetButtonColor(Colors color)
{
	m_outline->ChangeColor(color);
}

int GameEngine::Button::GetXPosition() const
{
	return m_xPos;
}

int GameEngine::Button::GetYPostion() const
{
	return m_yPos;
}

int GameEngine::Button::GetWidth() const
{
	return m_width;
}

int GameEngine::Button::GetHeight() const
{
	return m_height;
}

