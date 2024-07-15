#include "MessageBox.h"
#include "DrawUtils.h"

using namespace GameEngine;
using namespace DrawUtilities;

GameEngine::MessageBox::MessageBox()
{
	Initialize(11, 43, 433, 103, Colors::BLACK); // setup
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

	m_messageMap["default"] = std::make_unique<TextString>("F1: START GAME", m_x + 20, m_y + 16);
	m_messageMap["gameover"] = std::make_unique<TextString>("Game Over", m_x + 10, m_y + 30);

	m_message_01 = "";
	m_message_02 = "";
	m_message_03 = "";
    this->ChangeMessage(" ", "F1: START GAME");
	m_initialized = true;
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

    if (m_message_01.empty() && m_message_02.empty() && m_message_03.empty())
    {
        m_messageMap["default"]->DrawText(0.80f);
    }
    else
    {
        if (!m_message_01.empty())
        {
            m_messageMap[m_message_01]->DrawText(0.64f);
        }
        if (!m_message_02.empty())
        {
            m_messageMap[m_message_02]->DrawText(0.64f);
        }
        if (!m_message_03.empty())
        {
            m_messageMap[m_message_03]->DrawText(0.64f);
        }
    }
}