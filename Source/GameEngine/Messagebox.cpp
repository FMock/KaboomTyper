#include "MessageBox.h"

using namespace GameEngine;

GameEngine::MessageBox::MessageBox()
{
	Initialize(11, 43, 433, 103); // setup
}

GameEngine::MessageBox::MessageBox(int x, int y, int width, int height, Colors color)
{
	Initialize(x, y, width, height, color);
}

GameEngine::MessageBox::~MessageBox()
{
}

void GameEngine::MessageBox::Initialize(int x, int y, int width, int height, Colors color)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;

	bool fillMessageBox = true;
	m_body.Initialize(x, y, width, height, color, fillMessageBox);

	m_messageMap["default"] = std::make_unique<TextString>("Press F1 to Start", m_x + 20, m_y + 20);
	m_messageMap["gameover"] = std::make_unique<TextString>("Game Over", m_x + 10, m_y + 30);

	m_currentMessage = "";

	m_initialized = true;
}

void GameEngine::MessageBox::ChangeMessage(std::string message)
{
	m_currentMessage = message;
	m_messageMap[message] = std::make_unique<TextString>(message.c_str(), m_x + 20, m_y + 20);
}

void GameEngine::MessageBox::Draw()
{
	if (!m_initialized)
		throw std::exception("MessageBox not initialized");

	m_body.Draw();

	if(m_currentMessage.empty())
		m_messageMap["default"]->DrawText(0.80f);
	else
		m_messageMap[m_currentMessage]->DrawText(0.80);
}
