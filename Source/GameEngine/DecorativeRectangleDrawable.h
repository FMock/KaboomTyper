#pragma once

#include "RectangleDrawable.h"
#include <vector>
#include <chrono>
#include <thread>
#include <random>

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
        void SetAnimateRandom(bool animate);

    private:
        struct Rectangle
        {
            int x;
            int y;
            int width;
            int height;
        };

        GLuint GetRandomTexture(const std::vector<GLuint>& textures);
        int GetRandomIndex(const std::vector<Rectangle>& rectangles);
        void UpdateRandomIndicesAndColors(std::vector<int>& randomIndices, std::vector<GLuint>& randomColorTextures, const std::vector<Rectangle>& m_smallRectangles, const std::vector<GLuint>& textures);

        std::vector<Rectangle> m_smallRectangles;
        GLuint m_blueTexture;
        GLuint m_redTexture;
        GLuint m_yellowTexture;
        GLuint m_greenTexture;
        GLuint m_orangeTexture;

        float m_animationTime;
        float m_animationSpeed; // Speed of the animation
        float m_clockWiseAnimationSpeed;
        float m_randomAnimationSpeed;
        double m_randomAnimationTime;
        int m_lap;
        int m_currentColorIndex;
        bool m_animate;
        bool m_animateClockWise;
        bool m_animateRandom;
        float m_deltaTime;
        float m_threshold;

        const double ANIMATION_THRESHOLD = 0.001;

        int m_randomIndex;

        bool m_isVisible;
        float m_elapsedTime;

        std::chrono::steady_clock::time_point m_lastUpdateTime;   
        std::chrono::seconds m_updateInterval{ 1 };
    };
}