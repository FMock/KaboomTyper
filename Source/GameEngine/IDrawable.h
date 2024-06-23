#pragma once

#include "Color.h"
//#include "Colors.h"
#include <memory>

namespace GameEngine
{
    class IDrawable
    {
    public:
        virtual ~IDrawable() = default;
        virtual void Update(float dt) = 0;
        virtual void Draw() = 0;
        virtual int GetPriority() const = 0;
        virtual void SetPriority(int priority) = 0;

    protected:
        IDrawable() : m_x(0), m_y(0), m_color(GameEngine::Colors::DEFAULT_COLOR)
        {
            SetColorPtr(std::make_unique<Color>());
            CreateColorMap();
        }

        std::map<Colors, std::string> m_colorToStringMap;
        std::unique_ptr<Color> m_colorPtr; // derived classes use this to draw
        GameEngine::Colors m_color; // the specific color that will be drawn

        int m_x;
        int m_y;

        void SetX(int x)
        {
            m_x = x;
        }

        int GetX() const
        {
            return m_x;
        }

        void SetY(int y)
        {
            m_y = y;
        }

        int GetY() const
        {
            return m_y;
        }

    private:
        void SetColorPtr(std::unique_ptr<Color> color)
        {
            m_colorPtr = std::move(color);
        }

        // Function to convert enum values to strings
        inline std::string ColorToString(Colors color)
        {
            switch (color)
            {
            case RED: return "red";
            case DARK_RED: return "darkRed";
            case GREEN: return "green";
            case DARK_GREEN: return "darkGreen";
            case BLUE: return "blue";
            case DARK_BLUE: return "darkBlue";
            case PURPLE: return "purple";
            case DARK_PURPLE: return "darkPurple";
            case YELLOW: return "yellow";
            case DARK_YELLOW: return "darkYellow";
            case ORANGE: return "orange";
            case BROWN: return "brown";
            case GRAY: return "gray";
            case DARK_GRAY: return "darkGray";
            case WHITE: return "white";
            case BLACK: return "black";
            case UNKNOWN: return "unknown";
            case NONE: return "none";
            default: return "Invalid Color";
            }
        }

         // Function to create and initialize the map
        inline void CreateColorMap()
        {
            for (int i = RED; i <= NONE; ++i)
            {
                m_colorToStringMap[static_cast<Colors>(i)] = ColorToString(static_cast<Colors>(i));
            }
        }
    };
}
