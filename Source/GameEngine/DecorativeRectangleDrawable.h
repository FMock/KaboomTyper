// DecorativeRectangle.h
#pragma once

#include "RectangleDrawable.h"
#include <vector>

namespace GameEngine
{
    class DecorativeRectangle : public RectangleDrawable
    {
    public:
        using RectangleDrawable::RectangleDrawable; // Inherit constructors
        void Initialize(int x, int y, int width, int height, Colors rectColor = DEFAULT_COLOR, bool filled = false) override;
        void DrawRectangleWithRectangles();
        void InitializeDecorativeRectangle();

    private:
        struct Rectangle
        {
            int x;
            int y;
            int width;
            int height;
        };

        std::vector<Rectangle> m_smallRectangles;
    };
}