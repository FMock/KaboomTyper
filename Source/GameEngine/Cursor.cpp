#include "Cursor.h"
#include "DrawUtils.h"

using namespace GameEngine;
using namespace DrawUtilities;

void GameEngine::Cursor::Initialize(int x, int y, int width, int height, Colors rectColor, bool filled)
{
    m_elapsedTime = 0.0f;
    m_animationSpeed = 1.5f;
    m_animationThreshold = 1.0f;
    m_visible = true;

    auto color = m_colorToStringMap[rectColor];
    m_colorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color.c_str()];

    // Call the base class Initialize
    RectangleDrawable::Initialize(x, y, width, height, rectColor, filled);

    m_initialized = true;
}

void GameEngine::Cursor::Update(float dt)
{
    m_elapsedTime += dt * m_animationSpeed;

    if (m_elapsedTime >= m_animationThreshold)
    {
        m_elapsedTime -= m_animationThreshold;
        m_visible = !m_visible; // Toggle visibility
    }
}

void GameEngine::Cursor::Draw()
{
    if (m_visible)
    {
        DrawUtilities::glDrawTexture(m_colorTexture, m_x, m_y, (float)m_width, (float)m_height);
    }
}
