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
        void SetAnimateClockwise(bool animate);

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
        float m_animationTime;
        float m_animationSpeed; // Speed of the animation
        float m_clockWiseAnimationSpeed;
        int m_lap;
        int m_currentColorIndex;
        bool m_animate;
        bool m_animateClockWise;
        float m_deltaTime;
    };
}