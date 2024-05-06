#include "Button.h"

using namespace GameEngine;

Button::Button() : m_body(std::make_unique<RectangleDrawable>()), m_outline(std::make_unique<RectangleDrawable>()), m_label(std::make_unique<TextString>()), m_adjustedWidth(0), m_scaler(1.0f)
{

}

Button::Button(std::string text, int x, int y, float scaler, Colors color) : m_body(std::make_unique<RectangleDrawable>()), m_outline(std::make_unique<RectangleDrawable>()), m_adjustedWidth(0)
{
	Initialize(text, x, y, scaler, color);
}

Button::~Button()
{
}

void Button::Initialize(std::string text, int x, int y, float scaler, Colors color)
{
	m_scaler = scaler;
	m_adjustedWidth = static_cast<int>((text.size() * 24 * 1 * m_scaler));
	m_body->Initialize(x, y, m_adjustedWidth, 32 * m_scaler, color, true);
	m_outline->Initialize(x - 4, y - 4, m_adjustedWidth + 8, 32 * m_scaler + 8, Colors::DARK_GRAY);
	m_label->Initialize(text, x, y);
}

void Button::Update(float dt)
{
}

void Button::Draw()
{
	m_body->Draw();
	m_outline->Draw();
	m_label->DrawText(m_scaler);
}

void Button::ButtonPressed()
{
}

void Button::ButtonReleased()
{
}

void Button::SetButtonColor(Colors color)
{
	m_body->ChangeColor(color);
}

