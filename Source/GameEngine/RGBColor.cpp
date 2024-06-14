#include "RGBColor.h"

// Initialize the color array
const std::array<RGBColor, RGBColor::ColorCount> RGBColor::colors = {
    RGBColor(255, 255, 255), // White
    RGBColor(0, 0, 0),       // Black
    RGBColor(255, 0, 0),     // Red
    RGBColor(0, 255, 0),     // Green
    RGBColor(0, 0, 255),     // Blue
    RGBColor(255, 255, 0),   // Yellow
    RGBColor(0, 255, 255),   // Cyan
    RGBColor(255, 0, 255),   // Magenta
    RGBColor(128, 128, 128), // Gray
    RGBColor(139, 0, 0),     // DarkRed
    RGBColor(0, 100, 0),     // DarkGreen
    RGBColor(20, 20, 80),    // DarkBlue
    RGBColor(255, 165, 0),   // Orange
    RGBColor(255, 192, 203), // Pink
    RGBColor(128, 0, 128),   // Purple
    RGBColor(165, 42, 42),   // Brown
    RGBColor(0, 255, 0),     // Lime
    RGBColor(128, 128, 0),   // Olive
    RGBColor(0, 0, 128),     // Navy
    RGBColor(0, 128, 128),   // Teal
    RGBColor(128, 0, 0),     // Maroon
    RGBColor(192, 192, 192), // Silver
    RGBColor(255, 215, 0),   // Gold
    RGBColor(173, 216, 230), // LightBlue
    RGBColor(144, 238, 144), // LightGreen
    RGBColor(211, 211, 211), // LightGray
    RGBColor(60, 60, 60)     // DarkGray
};

const RGBColor& RGBColor::GetRGBColor(ColorName colorName)
{
    return colors[colorName];
}
