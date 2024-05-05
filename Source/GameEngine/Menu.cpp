#include "Menu.h"

using namespace GameEngine;

Menu::Menu() 
	: m_menuBar(std::make_unique<RectangleDrawable>()), m_optionsBtn(std::make_unique<Button>()), m_aboutBtn(std::make_unique<Button>()), m_helpBtn(std::make_unique<Button>()),
      m_fileBtn(std::make_unique<Button>())
{
	Initialize();
}

Menu::~Menu()
{
}

void Menu::Draw()
{
	m_menuBar->Draw();
	m_fileBtn->Draw();
	m_optionsBtn->Draw();
	m_helpBtn->Draw();
	m_aboutBtn->Draw();
}

void Menu::Update()
{
}

void GameEngine::Menu::Initialize()
{
	m_menuBar->Initialize(0, 0, 900, 34, Colors::YELLOW, true);
	m_fileBtn->Initialize("FILE", 5, 8, 0.65f, Colors::DARK_YELLOW);
	m_optionsBtn->Initialize("OPTIONS", 82, 8, 0.65f, Colors::DARK_YELLOW);
	m_helpBtn->Initialize("HELP", 210, 8, 0.65f, Colors::DARK_YELLOW);
	m_aboutBtn->Initialize("ABOUT", 290, 8, 0.65f, Colors::DARK_YELLOW);

}
