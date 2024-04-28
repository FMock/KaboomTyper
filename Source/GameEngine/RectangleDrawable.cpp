#include "RectangleDrawable.h"
#include "DrawUtils.h"
#include <cmath>
#include <vector>
#include "AudioGenerator.h"

using namespace DrawUtilities;

namespace GameEngine
{
    RectangleDrawable::RectangleDrawable() : IDrawable(), m_initialized(false), m_width(0), m_height(0), m_colorTexture(0), m_colorTextureWidth(1), m_colorTextureHeight(1)
    {
        SetX(0);
        SetY(0);
        m_color = Colors::DEFAULT_COLOR;
    }

    RectangleDrawable::RectangleDrawable(int x, int y, int width, int height, Colors rectColor)
        : IDrawable(), m_width(width), m_height(height), m_initialized(true), m_colorTexture(0), m_colorTextureWidth(1), m_colorTextureHeight(1)
    {
        SetX(x);
        SetY(y);
        m_color = rectColor;
    }

    RectangleDrawable::~RectangleDrawable()
    {
        // Destructor implementation if needed
    }

    void RectangleDrawable::Initialize(int x, int y, int width, int height, Colors rectColor)
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
        m_initialized = true;
    }

    void RectangleDrawable::Update()
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

    void RectangleDrawable::DrawRectangle()
    {
        if (!m_initialized)
            throw std::exception("RectangleDrawable Not Fully Initialized");

        glDrawSpriteScaled(m_colorTexture, m_x, m_y, m_colorTextureWidth, m_colorTextureHeight, (float)m_width, (float)m_height);
    }
}
