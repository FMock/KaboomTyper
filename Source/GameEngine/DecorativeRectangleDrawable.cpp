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
    m_smallRectangles.clear();

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

void DecorativeRectangle::DrawRectangleWithRectangles()
{
    for (const auto& rect : m_smallRectangles)
    {
        DrawUtilities::glDrawRectangleOutline(m_colorTexture, rect.x, rect.y, (float)rect.width, (float)rect.height);
    }
}
