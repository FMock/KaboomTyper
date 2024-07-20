#include "MenuItem.h"
#include "DrawUtils.h"

using namespace GameEngine;
using namespace DrawUtilities;


MenuItem::MenuItem() : m_outline(std::make_unique<RectangleDrawable>()), m_label(std::make_unique<TextString>()),
                       m_adjustedWidth(0), m_scaler(1.0f), m_xPos(0), m_yPos(0), m_width(0), m_height(0), m_outlineWidth(0), m_outlineHeight(0), m_active(false)
{

}

MenuItem::MenuItem(std::string text, int x, int y, int parentX, int parentY, int parentWidth, float scaler, Colors color) : 
	               m_outline(std::make_unique<RectangleDrawable>()), m_adjustedWidth(0), m_outlineWidth(0), m_outlineHeight(0), m_active(false)
{
	Initialize(text, x, y, parentX, parentY, parentWidth, scaler, color);
}

MenuItem::~MenuItem()
{
}

void MenuItem::Initialize(std::string text, int x, int y, int parentX, int parentY, int parentWidth, float scaler, Colors color)
{
	m_xPos = parentX;
	m_yPos = y - Common::MENUITEM_POSTION_OFFSET;

	m_scaler = scaler;
	int scaledTextureHeight = static_cast<int>(32 * m_scaler); // 32 is font texture height
	m_adjustedWidth = static_cast<int>((text.size() * 24 * 1 * m_scaler)); // 24 is font texture width
	m_width = m_adjustedWidth + (2 * Common::MENUITEM_POSTION_OFFSET);
	m_height = scaledTextureHeight + (2 * Common::MENUITEM_POSTION_OFFSET);
	m_outlineWidth = parentWidth - 10;
	m_outlineHeight = static_cast<int>(32 * m_scaler) + 8;
	m_outline->Initialize(m_xPos, m_yPos, m_outlineWidth, m_outlineHeight, Colors::DEFAULT_COLOR);
	m_label->Initialize(text, x, y);
}

void MenuItem::Update(float dt)
{
}

void MenuItem::Draw()
{
	if (m_active)
	{
		m_outline->Draw();

		RGBColor color = RGBColor::GetRGBColor(RGBColor::White);
		glDrawFilledRectangle(m_xPos, m_yPos, m_outlineWidth, m_outlineHeight, 1.0f, 1.0f, color, 50);
	}

	m_label->DrawText(m_scaler);
}

bool MenuItem::GetIsActive() const
{
	return m_active;
}

void MenuItem::SetIsActive(bool isActive)
{
	m_active = isActive;
}

void GameEngine::MenuItem::SetLabelText(const std::string& text)
{
	m_label->SetText(text);
}

std::string GameEngine::MenuItem::GetLabelText() const
{
	return m_label->GetText();
}

bool MenuItem::IsMouseOverMenuItem(const int& x, const int& y)
{
	return x >= m_xPos && x <= m_xPos + m_outlineWidth &&
		y >= m_yPos && y <= m_yPos + m_outlineHeight;
}

void MenuItem::SetMenuItemColor(Colors color)
{
	m_outline->ChangeColor(color);
}

int GameEngine::MenuItem::GetXPosition() const
{
	return m_xPos;
}

int GameEngine::MenuItem::GetYPostion() const
{
	return m_yPos;
}

int GameEngine::MenuItem::GetWidth() const
{
	return m_width;
}

int GameEngine::MenuItem::GetHeight() const
{
	return m_height;
}

