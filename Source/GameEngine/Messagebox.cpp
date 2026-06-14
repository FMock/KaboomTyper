#include "MessageBox.h"
#include "DrawUtils.h"

using namespace GameEngine;
using namespace DrawUtilities;

GameEngine::MessageBox::MessageBox()
{
	Initialize(11, 43, 533, 103, Colors::BLACK); // widened to fill up to the scoreboard's new position
}

GameEngine::MessageBox::MessageBox(int x, int y, int width, int height, Colors color)
{
	Initialize(x, y, width, height, color);
}


void GameEngine::MessageBox::Initialize(int x, int y, int width, int height, Colors color)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;

	bool fillMessageBox = true;
	m_body.Initialize(x, y, width, height, color, fillMessageBox);

	m_defaultMessage = std::make_unique<TextString>("F1: START GAME", m_x + LINE_X_PADDING, m_y + LINE_Y_START);

	// Start screen: the start prompt plus the menu navigation shortcuts. Labels are
	// space-padded (monospaced font) so everything after the ':' lines up in one column.
	this->ChangeMessage("F1:       START GAME",
						 "OPTIONS:  CTRL + O",
						 "HELP:     CTRL + H",
						 "ESC:      CLOSE MENUS");
	m_initialized = true;
}

void GameEngine::MessageBox::SetBanner(const std::string& text)
{
	if (text.empty())
		m_banner.reset();
	else
		m_banner = std::make_unique<TextString>(text.c_str(), m_x + BANNER_X, m_y + BANNER_Y);
}

void GameEngine::MessageBox::Update(float dt)
{
}

void GameEngine::MessageBox::Draw()
{
    if (!m_initialized)
        throw std::exception("MessageBox not initialized");

    m_body.Draw();

    // Background color of the MessageBox
    RGBColor color = RGBColor::GetRGBColor(RGBColor::DarkBlue);
    glDrawFilledRectangle(m_x, m_y, m_width, m_height, 1.0f, 1.0f, color);

    if (m_lines.empty())
    {
        m_defaultMessage->DrawText(0.80f);
    }
    else
    {
        for (auto& line : m_lines)
        {
            line->DrawText(LINE_SCALE);
        }
    }

    if (m_banner)
    {
        m_banner->DrawText(BANNER_SCALE);
    }
}