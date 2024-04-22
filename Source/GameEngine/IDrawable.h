#pragma once

#include "Color.h"
#include <memory>

namespace GameEngine
{
    class IDrawable
    {
    public:
        virtual ~IDrawable() = default;
        virtual void Update() = 0;
        virtual void Draw() = 0;

    protected:
        IDrawable()
        {
            SetColor(std::make_unique<Color>());
        }

        std::unique_ptr<Color> m_color; // derived classes use this to draw

        void SetColor(std::unique_ptr<Color> color)
        {
            m_color = std::move(color);
        }
    };
}
