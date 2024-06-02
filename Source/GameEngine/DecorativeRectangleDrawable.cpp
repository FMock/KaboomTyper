//// DecorativeRectangle.cpp
//#include "DecorativeRectangleDrawable.h"
//#include "DrawUtils.h"
//
//using namespace GameEngine;
//using namespace DrawUtilities;
//
//void DecorativeRectangle::Initialize(int x, int y, int width, int height, Colors rectColor, bool filled)
//{
//    // Call the base class Initialize
//    RectangleDrawable::Initialize(x, y, width, height, rectColor, filled);
//
//    // Call the specialized initialization
//    InitializeDecorativeRectangle();
//}
//
//void DecorativeRectangle::InitializeDecorativeRectangle()
//{
//    auto color_1 = m_colorToStringMap[Colors::BLUE];
//    auto color_2 = m_colorToStringMap[Colors::RED];
//
//    m_secondColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_1.c_str()];
//    m_thirdColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_2.c_str()];
//
//    m_smallRectangles.clear();
//
//    int smallWidth = 10; // width of smaller rectangles
//    int smallHeight = 10; // height of smaller rectangles
//
//    // Calculate number of rectangles needed for each side
//    int horizontalCount = m_width / smallWidth;
//    int verticalCount = m_height / smallHeight;
//
//    // Top side
//    for (int i = 0; i < horizontalCount; ++i)
//    {
//        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y, smallWidth, smallHeight });
//    }
//
//    // Bottom side
//    for (int i = 0; i < horizontalCount; ++i)
//    {
//        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y + m_height - smallHeight, smallWidth, smallHeight });
//    }
//
//    // Left side
//    for (int i = 0; i < verticalCount; ++i)
//    {
//        m_smallRectangles.push_back({ m_x, m_y + i * smallHeight, smallWidth, smallHeight });
//    }
//
//    // Right side
//    for (int i = 0; i < verticalCount; ++i)
//    {
//        m_smallRectangles.push_back({ m_x + m_width - smallWidth, m_y + i * smallHeight, smallWidth, smallHeight });
//    }
//}
//
//void DecorativeRectangle::Update(float dt)
//{
//    // update members for animation
//}
//
//void DecorativeRectangle::SetAnimate(bool animate)
//{
//    m_animate = animate;
//}
//
//void DecorativeRectangle::DrawRectangleWithRectangles()
//{
//    for (const auto& rect : m_smallRectangles)
//    {
//       glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
//
//       if (m_animate)
//       {
//           glDrawTexture(m_secondColorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
//           glDrawTexture(m_thirdColorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
//       }
//    }
//}


// DecorativeRectangle.cpp
#include "DecorativeRectangleDrawable.h"
#include "DrawUtils.h"

using namespace GameEngine;
using namespace DrawUtilities;

void DecorativeRectangle::Initialize(int x, int y, int width, int height, Colors rectColor, bool filled)
{
    // Call the base class Initialize
    RectangleDrawable::Initialize(x, y, width, height, rectColor, filled);

    // Call the specialized initialization
    InitializeDecorativeRectangle();
}

void DecorativeRectangle::InitializeDecorativeRectangle()
{
    m_animationTime = 0.0f;
    m_animationSpeed = 1.50f; // Speed of the animation
    m_currentColorIndex = 0;
    m_animate = false;
    m_smallRectangles.clear();

    auto color_1 = m_colorToStringMap[Colors::BLUE];
    auto color_2 = m_colorToStringMap[Colors::RED];

    m_secondColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_1.c_str()];
    m_thirdColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_2.c_str()];

    int smallWidth = 10; // Example width of smaller rectangles
    int smallHeight = 10; // Example height of smaller rectangles

    // Calculate number of rectangles needed for each side
    int horizontalCount = m_width / smallWidth;
    int verticalCount = m_height / smallHeight;

    // Top side
    for (int i = 0; i < horizontalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y, smallWidth, smallHeight });
    }

    // Bottom side
    for (int i = 0; i < horizontalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y + m_height - smallHeight, smallWidth, smallHeight });
    }

    // Left side
    for (int i = 0; i < verticalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x, m_y + i * smallHeight, smallWidth, smallHeight });
    }

    // Right side
    for (int i = 0; i < verticalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x + m_width - smallWidth, m_y + i * smallHeight, smallWidth, smallHeight });
    }
}

void DecorativeRectangle::Update(float dt)
{
    if (m_animate)
    {
        m_animationTime += dt * m_animationSpeed;

        // Change color every second (for example)
        if (m_animationTime >= 1.0f)
        {
            m_animationTime = 0.0f;
            m_currentColorIndex = (m_currentColorIndex + 1) % 2; // Alternate between 0 and 1
        }
    }
}

void DecorativeRectangle::SetAnimate(bool animate)
{
    m_animate = animate;
}

void DecorativeRectangle::DrawRectangleWithRectangles()
{
    for (const auto& rect : m_smallRectangles)
    {
        // Draw the small rectangles as alternating colors
        if (m_animate)
        {
            GLuint textureToUse = (m_currentColorIndex == 0) ? m_secondColorTexture : m_thirdColorTexture;
            DrawUtilities::glDrawTexture(textureToUse, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
        else
        {
            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
    }
}

