#include "RectangleDrawable.h"
#include "DrawUtils.h"
#include <cmath>
#include <vector>
#include "AudioGenerator.h"
#include "RGBColor.h"

using namespace DrawUtilities;

namespace GameEngine
{
    RectangleDrawable::RectangleDrawable() : IDrawable(), m_initialized(false), m_width(0), m_height(0), m_colorTexture(0), m_colorTextureWidth(1), m_colorTextureHeight(1)
    {
        SetX(0);
        SetY(0);
        m_color = Colors::DEFAULT_COLOR;
    }

    RectangleDrawable::RectangleDrawable(int x, int y, int width, int height, Colors rectColor, bool filled)
        : IDrawable(), m_width(width), m_height(height), m_initialized(true), m_colorTexture(0), m_colorTextureWidth(1), m_colorTextureHeight(1)
    {
        SetX(x);
        SetY(y);
        m_color = rectColor;
    }

    void RectangleDrawable::Initialize(int x, int y, int width, int height, Colors rectColor, bool filled)
    {
        SetX(x);
        SetY(y);
        m_width = width;
        m_height = height;
        m_color = rectColor;
        m_colorTextureWidth = m_colorPtr->s_colorParameters.textureWidth;  // m_colorPtr has GLuint textures that are 1 pixel wide
        m_colorTextureHeight = m_colorPtr->s_colorParameters.textureHeight; // m_colorPtr has GLuint textres that are 1 pixel high
        auto color = m_colorToStringMap[m_color];
        m_colorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color.c_str()];
        m_filled = filled;
        m_initialized = true;
    }

    void RectangleDrawable::Update(float dt)
    {
        // Update logic for RectangleDrawable
    }

    void RectangleDrawable::Draw()
    {
        if (!m_initialized)
            throw std::exception("RectangleDrawable Not Fully Initialized");

        DrawRectangle();
    }

    void RectangleDrawable::SetWidth(int width)
    {
        m_width = width;
    }

    int RectangleDrawable::GetWidth() const
    {
        return m_width;
    }

    void RectangleDrawable::SetHeight(int height)
    {
        m_height = height;
    }

    int RectangleDrawable::GetHeight() const
    {
        return m_height;
    }

    void RectangleDrawable::SetXPos(int x)
    {
        m_x = x;
    }

    void RectangleDrawable::DrawRectangle()
    {
        if (!m_initialized)
            throw std::exception("RectangleDrawable Not Fully Initialized");

        if (m_filled)
            glDrawTexture(m_colorTexture, m_x, m_y, (float)m_width, (float)m_height);
        else
            glDrawRectangleOutline(m_colorTexture, m_x, m_y, (float)m_width, (float)m_height);
    }

    void RectangleDrawable::ChangeColor(Colors newColor)
    {
        m_color = newColor;
        auto color = m_colorToStringMap[m_color];
        m_colorTexture = m_colorPtr->s_colorParameters.m_stringColorTextureColorMap[color.c_str()];
    }
}
