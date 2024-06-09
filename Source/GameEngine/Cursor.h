#pragma once
#include "RectangleDrawable.h"

/// <summary>
/// A flashing cursor that indicates the location of the next text character
/// </summary>

namespace GameEngine
{
    class Cursor : public RectangleDrawable
    {
    public:
        using RectangleDrawable::RectangleDrawable; // Inherit constructors
        void Initialize(int x, int y, int width, int height, Colors rectColor, bool filled) override;
        void Update(float dt) override;
        void Draw() override;

    private:
        float m_elapsedTime;
        float m_animationSpeed;
        float m_animationThreshold;
        bool m_visible;
    };
}
