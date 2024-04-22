#include "RectangleDrawable.h"
#include "DrawUtils.h"

using namespace DrawUtilities;

namespace GameEngine
{
    RectangleDrawable::RectangleDrawable()
    {
        // The base class constructor will initialize m_color
    }

    RectangleDrawable::~RectangleDrawable()
    {
        // Destructor implementation if needed
    }

    void RectangleDrawable::Update()
    {
        // Update logic for RectangleDrawable
    }

    void RectangleDrawable::Draw()
    {
        DrawRectangle();
    }
    void RectangleDrawable::DrawRectangle()
    {
        int xPostion = 100;
        int yPosition = 100;
        int width = 1;  // m_color has GLuint textures that are 1 pixel wide
        int height = 1; // m_color has GLuint textres that are 1 pixel wide

        glDrawSpriteScaled(m_color->s_colorParameters.m_stringColorTextureColorMap["red"], xPostion, yPosition, width, height, 32.0f, 32.0f);
    }
}
