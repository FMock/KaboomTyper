#include "GamePlayArea.h"
#include "DrawUtils.h"
#include "Common.h"

using namespace GameEngine;
using namespace DrawUtilities;

void GamePlayArea::Initialize()
{
    m_leftSideBar.Initialize(0, 42, 10, 908, Colors::BLUE, true);
    m_rightSideBar.Initialize(790, 42, 10, 908, Colors::BLUE, true);
    m_topSideBar.Initialize(0, 146, 800, 5, Colors::BLUE, true);
    m_bottomSideBar.Initialize(0, 910, 800, 5, Colors::BLUE, true);
}

void GamePlayArea::Render()
{
    // Background color of the play area
    RGBColor color = RGBColor::GetRGBColor(RGBColor::DarkBlue);
    glDrawFilledRectangle(10, 152, 780, 760, 1.0f, 1.0f, color);

    // Red TextBox limit triangles on each side of play area
    RGBColor triangleColor = RGBColor::GetRGBColor(RGBColor::Red);
    glDrawFilledTriangle(0, Common::CEILING, 50, 1.0f, 1.0f, triangleColor, -90.0f);
    glDrawFilledTriangle(800, Common::CEILING, 50, 1.0f, 1.0f, triangleColor, 90.0f);

    // Blue frame around game area
    m_leftSideBar.Draw();
    m_rightSideBar.Draw();
    m_topSideBar.Draw();
    m_bottomSideBar.Draw();
}
