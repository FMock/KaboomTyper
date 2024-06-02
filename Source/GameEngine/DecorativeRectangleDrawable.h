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
        void Update(float dt) override;
        void SetAnimate(bool animate);

    private:
        struct Rectangle
        {
            int x;
            int y;
            int width;
            int height;
        };

        std::vector<Rectangle> m_smallRectangles;
        GLuint m_secondColorTexture;
        GLuint m_thirdColorTexture;
        float m_animationTime = 0.0f;
        float m_animationSpeed = 1.0f; // Speed of the animation
        int m_currentColorIndex = 0;
        bool m_animate;
    };
}