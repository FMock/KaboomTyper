#include "ChoiceMenuItem.h"
#include "DrawUtils.h"
#include <iostream>

using namespace GameEngine;
using namespace DrawUtilities;

ChoiceMenuItem::ChoiceMenuItem() : m_label(std::make_unique<TextString>()), m_augmentedLabel(std::make_unique<TextString>()),m_clickBox(std::make_unique<Button>()),
                                   m_labelScaler(0.6f), m_width(0), m_height(0), m_xPos(0), m_yPos(0), m_active(false), m_isInitialized(false)
{
    Initialize("ChoiceMenuItem", 0, 0, m_labelScaler);
}

ChoiceMenuItem::~ChoiceMenuItem() = default;

ChoiceMenuItem::ChoiceMenuItem(std::string text, int x, int y, float labelScaler) : m_label(std::make_unique<TextString>()), m_augmentedLabel(std::make_unique<TextString>()),
                                                                                                  m_clickBox(std::make_unique<Button>()), m_labelScaler(1.0f), 
                                                                                                  m_width(0), m_height(0), m_xPos(x), m_yPos(y), m_active(false), m_name(text)
{
    Initialize(text, x, y, labelScaler);
}

void ChoiceMenuItem::Initialize(const std::string& text, int x, int y, float labelScaler)
{
    int pad = 20;

    // initialize label, a.k. TextString
    m_label->Initialize(text, x + pad, y + 3);
    m_augmentedLabel->Initialize(text, x + pad, y + 3);
    auto labelWidth = (text.length() * (24 * m_labelScaler));

    // initialize clickBox, a.k.a Button
    m_clickBoxX = x + labelWidth + (pad * 2);
    m_clickBoxY = y + 3;
    m_clickBox->Initialize(m_selectX, m_clickBoxX + (m_augmentSize*24*m_labelScaler), m_clickBoxY, m_labelScaler, Colors::DEFAULT_COLOR);
    m_width = labelWidth + (2*24)+ 24 + (m_augmentSize * 24 * m_labelScaler);
    m_height = 32;
    m_isInitialized = true;
}

void GameEngine::ChoiceMenuItem::Update(float dt)
{
}

void GameEngine::ChoiceMenuItem::Draw()
{
    //m_label->DrawText(m_labelScaler);
    m_augmentedLabel->DrawText(m_labelScaler);
    m_clickBox->Draw();
}

bool GameEngine::ChoiceMenuItem::MouseHoverPresent(const int& x, const int& y)
{
    return x >= m_xPos && x <= m_xPos + m_width && y >= m_yPos && y <= m_yPos + m_height;
}

int GameEngine::ChoiceMenuItem::GetXPosition() const
{
    return m_xPos;
}

void GameEngine::ChoiceMenuItem::SetXPosition(int x)
{
    m_xPos = x;
}

int GameEngine::ChoiceMenuItem::GetYPostion() const
{
    return m_yPos;
}

void GameEngine::ChoiceMenuItem::SetYPosition(int y)
{
    m_yPos = y;
}

int GameEngine::ChoiceMenuItem::GetWidth() const
{
    return m_width;
}

int GameEngine::ChoiceMenuItem::GetHeight() const
{
    return m_height;
}

bool GameEngine::ChoiceMenuItem::GetIsActive() const
{
    return m_active;
}

void GameEngine::ChoiceMenuItem::SetIsActive(bool isActive)
{
    m_active = isActive;
}

bool GameEngine::ChoiceMenuItem::GetIsSelected()
{
    return m_isSelected;
}

void GameEngine::ChoiceMenuItem::SetIsSelected(bool isSelected)
{
    m_isSelected = isSelected;
    m_selectX = isSelected ? "x" : " ";
    m_clickBox->SetText(m_selectX);
}

std::string GameEngine::ChoiceMenuItem::GetName() const
{
    return m_name;
}

void GameEngine::ChoiceMenuItem::SetName(std::string name)
{
    m_name = name;
}

void GameEngine::ChoiceMenuItem::AugmentLabel(int n)
{
    std::string text = m_augmentedLabel->GetText();

    // Append n asterisks at the end of text
    text.append(n, '-');

    // Optionally, set the augmented text back to m_augmentedLabel if needed
    m_augmentedLabel->SetText(text);
}

float GameEngine::ChoiceMenuItem::GetLableScaler() const
{
    return m_labelScaler;
}

void GameEngine::ChoiceMenuItem::SetAugmentedSize(int s)
{
    m_augmentSize = s;
}
