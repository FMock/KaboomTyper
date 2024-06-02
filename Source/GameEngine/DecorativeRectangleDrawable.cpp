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
    m_clockWiseAnimationSpeed = 1.5f;
    m_currentColorIndex = 0;
    m_animate = false;
    m_smallRectangles.clear();
    m_lap = 0;

    auto color_1 = m_colorToStringMap[Colors::BLUE];
    auto color_2 = m_colorToStringMap[Colors::RED];

    m_secondColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_1.c_str()];
    m_thirdColorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color_2.c_str()];

    int smallWidth = 10; // width of smaller rectangles
    int smallHeight = 10; // height of smaller rectangles

    // Calculate number of rectangles needed for each side
    int horizontalCount = m_width / smallWidth;
    int verticalCount = m_height / smallHeight;

    // Top side
    for (int i = 0; i < horizontalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y, smallWidth, smallHeight });
    }

    // Right side
    for (int i = 0; i < verticalCount; ++i)
    {
        m_smallRectangles.push_back({ m_x + m_width - smallWidth, m_y + i * smallHeight, smallWidth, smallHeight });
    }

    // Bottom side
    for (int i = horizontalCount - 1; i >= 0; --i)
    {
        m_smallRectangles.push_back({ m_x + i * smallWidth, m_y + m_height - smallHeight, smallWidth, smallHeight });
    }

    // Left side
    for (int i = verticalCount - 1; i >= 0; --i)
    {
        m_smallRectangles.push_back({ m_x, m_y + i * smallHeight, smallWidth, smallHeight });
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

    if (m_animateClockWise)
    {
        static int currentIndex = 0; // Index of the current small rectangle
        static float elapsedTime = 0.0f; // Time elapsed since the last change

        // Use delta time to control the animation speed
        elapsedTime += dt * m_clockWiseAnimationSpeed;

        // Adjust the threshold for when to move to the next rectangle
        float threshold = 1.0f;

        if (elapsedTime >= threshold)
        {
            elapsedTime = 0.0f;
            currentIndex = (currentIndex + 1) % m_smallRectangles.size();

            // Check if we've completed a full lap
            if (currentIndex == 0)
            {
                ++m_lap;
            }
        }
    }
}

void DecorativeRectangle::SetAnimate(bool animate)
{
    m_animate = animate;
}

void GameEngine::DecorativeRectangle::SetAnimateClockwise(bool animate)
{
    m_animateClockWise = animate;
}

void DecorativeRectangle::DrawRectangleWithRectangles()
{
    static int currentIndex = 0; // Index of the current small rectangle
    static float elapsedTime = 0.0f; // Time elapsed since the last change

    if (m_animateClockWise)
    {
        elapsedTime += 0.001f * m_clockWiseAnimationSpeed;

        if (elapsedTime >= 1.0f)
        {
            elapsedTime = 0.0f;
            currentIndex = (currentIndex + 1) % m_smallRectangles.size();

            // Check if we've completed a full lap
            if (currentIndex == 0)
            {
                ++m_lap;
            }
        }

        // Draw the current rectangle with a different color
        const auto& rect = m_smallRectangles[currentIndex];
        DrawUtilities::glDrawTexture(m_secondColorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);

        for (const auto& rect : m_smallRectangles)
        {
            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
    }
    else if(m_animate)
    {
        for (const auto& rect : m_smallRectangles)
        {
            //if (m_animate)
            //{
                GLuint textureToUse = (m_currentColorIndex == 0) ? m_secondColorTexture : m_thirdColorTexture;
                DrawUtilities::glDrawTexture(textureToUse, rect.x, rect.y, (float)rect.width, (float)rect.height);
            //}

            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
    }
    else
    {
        for (const auto& rect : m_smallRectangles)
        {
            DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
        }
    }
}
